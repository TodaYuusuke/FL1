#include "MoveController.h"
#include "Action/Move/Move.h"
#include "Action/Boost/Boost.h"
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
		if(it->second) it->second->Init();
	}
}

void MoveController::Update() {
	// 入力処理
	InputHandle();

	for (auto it = actions_.begin(); it != actions_.end(); ++it) {
		if (it->second) it->second->Update();
	}

	Vector3 vel{};
	// 回避時
	if(actions_[ActionType::kSub]->GetStateName() == "Boost") {
		vel = actions_[ActionType::kMain]->GetVel() * actions_[ActionType::kMain]->GetMoveSpeed() * actions_[ActionType::kSub]->GetMoveSpeed();
	}
	else {
		vel = actions_[ActionType::kMain]->GetVel() * actions_[ActionType::kMain]->GetMoveSpeed() + actions_[ActionType::kSub]->GetVel() * actions_[ActionType::kSub]->GetMoveSpeed();
	}

	vel_ = vel;
	rot_ = actions_[ActionType::kMain]->GetRot();

	// 移動ベクトルの長さが一定値以上かつブースト中以外なら
	if (std::abs(actions_[ActionType::kMain]->GetRawVel().Length()) > 0.05f && SEPlayer::GetInstance()->GetAudioPlayer(moveSEID_) == nullptr && actions_[ActionType::kSub]->GetStateName() != "Boost") {
		// 移動音再生
		moveSEID_ = SEPlayer::GetInstance()->PlaySE("move_SE.mp3", 1.0f, LWP::AudioConfig::Player, true);
	}
	else if (std::abs(actions_[ActionType::kMain]->GetRawVel().Length()) <= 0.05f && SEPlayer::GetInstance()->GetAudioPlayer(moveSEID_) != nullptr) {
		// ループ音再生停止
		if (AudioPlayer* p = SEPlayer::GetInstance()->GetAudioPlayer(moveSEID_)) {
			p->Stop(0.5f);
		}
	}

	// ブースト時のエフェクトのかかり具合を設定する
	if (vCon_->GetPress(BindActionType::kBoost)) {
		float speed = actions_[ActionType::kMain]->GetMoveSpeed() + actions_[ActionType::kSub]->GetMoveSpeed();
		float maxSpeed = 4.0f;

		// ビネットエフェクト
		CameraEffectHandler::GetInstance()->GetVignetteEffector()->SetMaxStrength(LWP::Utility::Interp::LerpF(0.0f, 0.5f, LWP::Utility::Easing::CallFunction(LWP::Utility::Easing::Type::Liner, std::clamp(speed / maxSpeed, 0.f, 1.f))));
		// ブラーエフェクト
		if (speed <= maxSpeed) {
			CameraEffectHandler::GetInstance()->GetBlurEffector()->SetMaxStrength(LWP::Utility::Interp::LerpF(0.0f, 0.005f, LWP::Utility::Easing::CallFunction(LWP::Utility::Easing::Type::Liner, std::clamp(speed / maxSpeed, 0.f, 1.f))));
		}
		else {
			// ブラーエフェクト終了
			cameraEffector_->GetBlurEffector()->SetMaxStrength(0.0f);
			cameraEffector_->GetBlurEffector()->Finish(0.5f);
		}
	}
}

void MoveController::DebugGui() {
	for (auto it = actions_.begin(); it != actions_.end(); ++it) {
		if (it->second) it->second->DebugGui();
	}
}

void MoveController::InputHandle() {
	// 回避
	bool isBoost = false;
	if (vCon_->GetPress(BindActionType::kBoost)) {
		if (CheckEnableChangeState(Movement::SubAction::boost, actions_[ActionType::kSub]->GetEnableChangeState())) {
			if (CheckInabilityParallelState(Movement::SubAction::boost, actions_[ActionType::kMain]->GetInabilityParallelState())) {
				ChangeState(actions_[ActionType::kSub], std::make_unique<Boost>(actions_[ActionType::kMain]->GetVel(), pBB_->GetValue<Actor*>("Player")->GetWorldTF()->GetWorldPosition()));
				isBoost = true;

				// カメラ演出開始
				if (vCon_->GetTrigger(BindActionType::kBoost)) {
					cameraEffector_->StartZoom(boostCameraFov, boostCameraEffectTime);
					cameraEffector_->StartShake(boostCameraShake, boostCameraEffectTime);
					cameraEffector_->StartBound(boostCameraBound, boostCameraBoundTime);

					// ループ音再生停止
					if (AudioPlayer* p = SEPlayer::GetInstance()->GetAudioPlayer(moveSEID_)) {
						p->Stop();
					}

					// ブラーエフェクト開始
					cameraEffector_->GetBlurEffector()->Start(0.5f);
					// ビネットエフェクト開始
					cameraEffector_->GetVignetteEffector()->Start(1.0f);

					// ブースト音も再生開始
					SEPlayer::GetInstance()->PlaySE("beginBoost_SE.mp3", 1.0f, LWP::AudioConfig::Player);
					boostSEID_ = SEPlayer::GetInstance()->PlaySE("boost_SE.mp3", 1.0f, LWP::AudioConfig::Player, true);
				}
			}
		}
	}
	if (!isBoost) {
		ChangeState(actions_[ActionType::kSub], std::make_unique<NoneAction>());

		// カメラ演出終了
		if (vCon_->GetRelease(BindActionType::kBoost)) {
			cameraEffector_->StartZoom(-boostCameraFov, boostCameraEffectTime * 2.0f);

			// ブラーエフェクト終了
			cameraEffector_->GetBlurEffector()->Finish(0.5f);

			// ビネットエフェクト終了
			cameraEffector_->GetVignetteEffector()->Finish(1.0f);

			// ループ音再生停止
			if (AudioPlayer* p = SEPlayer::GetInstance()->GetAudioPlayer(boostSEID_)) {
				p->Stop(0.5f);
			}
		}
	}
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

void MoveController::StopAllLoopSE()
{
	// ループ音再生停止
	if (AudioPlayer* p = SEPlayer::GetInstance()->GetAudioPlayer(boostSEID_)) {
		p->Stop();
	}
	if (AudioPlayer* p = SEPlayer::GetInstance()->GetAudioPlayer(moveSEID_)) {
		p->Stop();
	}
}
