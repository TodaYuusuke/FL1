#include "MoveController.h"
#include "Action/None/NoneAction.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/InputMyController/ControllerReceiver.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace LWP::Input;
using namespace LWP::Math;

MoveController::MoveController(BlackBoard* blackBoard) {
	pBB_ = blackBoard;
	vCon_ = VirtualController::GetInstance();
	cameraEffector_ = CameraEffectHandler::GetInstance();

	actions_[ActionType::kMain] = std::make_unique<Move>(pBB_);
	actions_[ActionType::kSub] = std::make_unique<NoneAction>();
	enableChangeState_ = Movement::SubAction::jump | Movement::SubAction::sliding | Movement::SubAction::boost;
}

MoveController::~MoveController() {

}

void MoveController::Init() {
	for (auto it = actions_.begin(); it != actions_.end(); ++it) {
		if (it->second) it->second->Init();
	}

	// 前フレーム角度取得
	preRot_ = rot_;
}

void MoveController::Update() {
	// 入力処理
	InputHandle();

	// 各機能の更新
	for (auto it = actions_.begin(); it != actions_.end(); ++it) {
		if (it->second) it->second->Update();
	}

	// 各機能で算出した速度を統合
	vel_ = actions_[ActionType::kMain]->GetVel() * actions_[ActionType::kMain]->GetMoveSpeed();

	// 角度
	rot_ = actions_[ActionType::kMain]->GetRot();
	// 演出用の角度
	effectRot_ = actions_[ActionType::kMain]->GetEffectRot();

	// ブースト時の音
	PlayBoostAudio();

	// ブースト時のエフェクトのかかり具合を設定する
	AdjustBoostEffect();

	// 前フレーム角度リセット
	preRot_ = rot_;
}

void MoveController::DebugGui() {
	for (auto it = actions_.begin(); it != actions_.end(); ++it) {
		if (it->second) it->second->DebugGui();
	}
}

void MoveController::InputHandle() {
	// ブースト
	if (vCon_->GetPress(BindActionType::kBoost)) {
		if (CheckEnableChangeState(Movement::SubAction::boost, actions_[ActionType::kSub]->GetEnableChangeState())) {
			if (CheckInabilityParallelState(Movement::SubAction::boost, actions_[ActionType::kMain]->GetInabilityParallelState())) {
				// 演出開始
				StartBoostEffect();
			}
		}
	}
	// ブースト終了
	if (vCon_->GetRelease(BindActionType::kBoost)) {
		// 演出終了
		FinishBoostEffect();
	}
}

/// 演出関数↓

void MoveController::PlayBoostAudio() {
	// 移動ベクトルの長さが一定値以上かつブースト中以外なら
	bool isPlayMoveSE =
		(std::abs(actions_[ActionType::kMain]->GetRawVel().Length()) > 0.05f && AudioPlayer::GetInstance()->GetAudioPlayer(moveSEID_) == nullptr && actions_[ActionType::kSub]->GetStateName() != "Boost");
	bool isStopMoveSE =
		(std::abs(actions_[ActionType::kMain]->GetRawVel().Length()) <= 0.05f && AudioPlayer::GetInstance()->GetAudioPlayer(moveSEID_) != nullptr);
	if (isPlayMoveSE) {
		// 移動音再生
		moveSEID_ = AudioPlayer::GetInstance()->PlayAudio("move_SE.mp3", 1.0f, LWP::AudioConfig::Player, true);
	}
	else if (isStopMoveSE) {
		// ループ音再生停止
		if (Sound* p = AudioPlayer::GetInstance()->GetAudioPlayer(moveSEID_)) {
			p->Stop(0.5f);
		}
	}
}

void MoveController::StartBoostEffect() {
	if (!vCon_->GetTrigger(BindActionType::kBoost)) { return; }

	// カメラの視野角を下げる(ズーム状態)
	cameraEffector_->StartZoom(boostCameraFov, boostCameraEffectTime);
	// カメラを揺らす
	cameraEffector_->StartShake(boostCameraShake, boostCameraEffectTime);
	// カメラを上下に跳ねさせる
	cameraEffector_->StartBound(boostCameraBound, boostCameraBoundTime);

	// ループ音再生停止
	if (Sound* p = AudioPlayer::GetInstance()->GetAudioPlayer(moveSEID_)) {
		p->Stop();
	}

	// ブラーエフェクト開始
	cameraEffector_->GetBlurEffector()->Start(0.5f);
	// ビネットエフェクト開始
	cameraEffector_->GetVignetteEffector()->Start(1.0f);

	// ブースト音も再生開始
	AudioPlayer::GetInstance()->PlayAudio("beginBoost_SE.mp3", 1.0f, LWP::AudioConfig::Player);
	boostSEID_ = AudioPlayer::GetInstance()->PlayAudio("boost_SE.mp3", 1.0f, LWP::AudioConfig::Player, true);
}

void MoveController::FinishBoostEffect() {
	// カメラ演出終了
	if (!vCon_->GetRelease(BindActionType::kBoost)) { return; }

	// もとの視野角に戻す
	cameraEffector_->StartZoom(-boostCameraFov, boostCameraEffectTime * 2.0f);

	// ブラーエフェクト終了
	cameraEffector_->GetBlurEffector()->Finish(0.5f);
	// ビネットエフェクト終了
	cameraEffector_->GetVignetteEffector()->Finish(1.0f);

	// ループ音再生停止
	if (Sound* p = AudioPlayer::GetInstance()->GetAudioPlayer(boostSEID_)) {
		p->Stop(0.5f);
	}
}

void MoveController::AdjustBoostEffect() {
	if (!vCon_->GetPress(BindActionType::kBoost)) { return; }
	float speed = actions_[ActionType::kMain]->GetMoveSpeed() + actions_[ActionType::kSub]->GetMoveSpeed();
	float maxSpeed = 4.0f;

	// ビネットエフェクト
	cameraEffector_->GetVignetteEffector()->SetMaxStrength(LWP::Utility::Interp::LerpF(0.0f, 0.5f, LWP::Utility::Easing::CallFunction(LWP::Utility::Easing::Type::Liner, std::clamp(speed / maxSpeed, 0.f, 1.f))));
	// ブラーエフェクト
	if (speed <= maxSpeed) {
		cameraEffector_->GetBlurEffector()->SetMaxStrength(LWP::Utility::Interp::LerpF(0.0f, 0.005f, LWP::Utility::Easing::CallFunction(LWP::Utility::Easing::Type::Liner, std::clamp(speed / maxSpeed, 0.f, 1.f))));
	}
	else {
		// ブラーエフェクト終了
		cameraEffector_->GetBlurEffector()->SetMaxStrength(0.0f);
		cameraEffector_->GetBlurEffector()->Finish(0.5f);
	}
}

Move* MoveController::GetMove() {
	if (!actions_[ActionType::kMain]) { return nullptr; }

	Move* result = dynamic_cast<Move*>(actions_[ActionType::kMain].get());
	return result;
}

bool MoveController::GetIsTurnBehind() {
	auto it = actions_.find(ActionType::kMain);
	if (it != actions_.end()) {
		if (auto move = dynamic_cast<Move*>(it->second.get())) {
			return move->GetIsTurnBehind();
		}
	}
	return false;
}

void MoveController::StopAllLoopSE() {
	// ループ音再生停止
	if (Sound* p = AudioPlayer::GetInstance()->GetAudioPlayer(boostSEID_)) {
		p->Stop();
	}
	if (Sound* p = AudioPlayer::GetInstance()->GetAudioPlayer(moveSEID_)) {
		p->Stop();
	}
}

/// 演出関数↑