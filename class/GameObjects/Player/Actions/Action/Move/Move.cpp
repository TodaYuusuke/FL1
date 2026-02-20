#include "Move.h"
#include "../../../../../Componets/Input/VirtualController.h"
#include "../../../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../../World/World.h"
#include "../../../../../Componets/InputMyController/ControllerReceiver.h"
#include "../../../../Camera/Effect/CameraEffectHandler.h"
#include <numbers>

using namespace FLMath;
using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Math;
using namespace LWP::Input;

Move::Move(BlackBoard* blackBoard) {
	pBB_ = blackBoard;
	stopController_ = HitStopController::GetInstance();
	stateName_ = "Move";

	json_.Init(kJsonFileDirectoryPath + "PlayerMove.json")
		// 車輪間の距離
		.AddValue<float>("TreadWidth", &treadWidth)
		// 移動速度の最高速
		.AddValue<float>("MaxSpeed", &maxSpeed)
		// 角速度の最高速
		.AddValue<float>("MaxOmega", &maxOmega)
		// 体の最大傾き[Degree]
		.AddValue<float>("MaxInclination", &maxInclination)
		// 旋回の閾値
		.AddValue<float>("RotThreshold", &rotThreshold)
		// 180度の閾値
		.AddValue<float>("TurnThreshold", &turnThreshold)
		// 180度旋回にかかる時間
		.AddValue<float>("TurnTime", &turnTime)
		// 入力を検出する範囲
		.AddValue<Vector2>("InputDetectionRange", &inputDetectionRange)
		.AddValue<float>("MaxBoostTime", &maxBoostTime)
		.AddValue<float>("MaxBoostSpeed", &end_)
		.CheckJsonFile();
}

void Move::Init() {
	turnRadian_ = {};
	preTurnRadian_ = {};
}

void Move::Update() {
	// 180度旋回入力
	isTurnBehind_ = false;
	// 移動方式選択
	CheckMoveType();

	// ブースト
	BoostUpdate();

	// 180度回転入力があった時の処理
	TurnBehind();
	turnTime_.Update();

	// 速度ベクトルから体の傾き算出
	BodyInclination();

	// 角速度算出
	//rot_ *= effectRot_ * preEffectRot_.Inverse();

	// 補正前移動ベクトルを取得
	rawVel_ = vel_;
	// タイムスケール適用
	vel_ *= stopController_->GetDeltaTime() * boostSpeed_;

	// 移動開始時にカメラを揺らす
	Vector2 v = (vR + vL) * 0.5f;
	if (v.Length() >= preVel_.Length()) {
		if (Vector3{ v.x, 0.0f, v.y } != preVel_) {
			if (std::fabsf(v.x) + std::fabsf(v.y) <= 0.05f) {
				if (std::fabsf(preVel_.x) + std::fabsf(preVel_.z) <= 0.01f) {
					CameraEffectHandler::GetInstance()->StartBound(Vector3{ 0.0f,0.01f,0.0f }, 0.05f);
				}
			}
		}
	}

	preVel_ = { v.x, 0.0f, v.y };
	preEffectRot_ = effectRot_;
	preTurnRadian_ = turnRadian_;
	isPreMoveTypeChange_ = isMoveTypeChange_;
	isPreTurnBehind_ = isTurnBehind_;
}

void Move::DebugGui() {
	if (ImGui::TreeNode("Move")) {
		if (ImGui::Button("TankMove")) {
			moveType_ = MoveType::kTank;
		}
		ImGui::SameLine();
		if (ImGui::Button("FPSMove")) {
			moveType_ = MoveType::kFPS;
		}

		// 調整項目
		json_.DebugGUI();

		if (ImGui::TreeNode("Current")) {
			ImGui::DragFloat("Omega", &omega);
			ImGui::DragFloat("Angle", &angle);
			ImGui::DragFloat("MoveSpeed", &moveSpeed_);
			ImGui::DragFloat("BoostSpeed", &boostSpeed_);
			ImGui::DragFloat3("Velocity", &vel_.x);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void Move::TurnBehind() {
	// 旋回開始
	if (isTurnBehind_ && !isPreTurnBehind_) {
		if (turnTime_.GetIsFinish()) {
			turnRadian_ = {};
			preTurnRadian_ = {};
			turnTime_.Start(turnTime);
			turnTime_.GetIsActive();
		}

		// 旋回音再生
		AudioPlayer::GetInstance()->PlayAudio("QuickTurn_SE.mp3", 1.0f, LWP::AudioConfig::Player, false);
	}

	if (!turnTime_.GetIsActive()) { return; }

	turnRadian_ = LWP::Utility::Interp::Lerp(Vector3{}, Vector3{ (float)std::numbers::pi, 0.0f, 0.0f }, turnTime_.GetProgress());

	// 前回の角度を引いて角速度を求める
	Vector3 radian = turnRadian_ - preTurnRadian_;
	rot_ *= Quaternion::CreateFromAxisAngle({ 0.0f,1.0f,0.0f }, radian.x);
}

void Move::BoostUpdate() {
	// 押した瞬間
	if (VirtualController::GetInstance()->GetTrigger(BindActionType::kBoost)) {
		easeTimer_.Start(maxBoostTime);
	}
	// 押している間
	else if (VirtualController::GetInstance()->GetPress(BindActionType::kBoost)) {
		boostSpeed_ = LWP::Utility::Interp::LerpF(start_, end_, LWP::Utility::Easing::OutBack(easeTimer_.GetProgress()));
	}
	// 離した瞬間
	else if (VirtualController::GetInstance()->GetRelease(BindActionType::kBoost)) {
		easeTimer_.Stop();
	}

	// 加速中ではないなら徐々に速度を落とす
	if (!easeTimer_.GetIsActive()) {
		boostSpeed_ = Exponential(boostSpeed_, 1.0f, easeRate);
	}

	// タイマー更新
	easeTimer_.Update();
}

void Move::DifferentialUpdate(LWP::Math::Vector2 leftStick, LWP::Math::Vector2 rightStick, float deltaTime) {
	// 履帯の目標速度（入力を反転したい場合は符号調整）
	Vector2 target_vL = leftStick * maxSpeed;
	Vector2 target_vR = rightStick * maxSpeed;

	// 値の修正される前のスティックの入力値で比較
	Vector2 lStick = VirtualController::GetInstance()->GetLAxis();
	Vector2 rStick = VirtualController::GetInstance()->GetRAxis();
	Vector2 sqrtStick = (lStick - rStick);

	// 片方の入力が無かったら0にする[Y軸方向]
	if (target_vL.y == 0.0f) {
		target_vR.y = 0.0f;
	}
	if (target_vR.y == 0.0f) {
		target_vL.y = 0.0f;
	}

	// スティック入力が互いに反対(クイックターンの判定)
	// 左右に入力しているか
	if (target_vL.x * target_vR.x < 0.0f) {
		if (std::abs(sqrtStick.x) >= turnThreshold * 2.0f) {
			target_vL.x = 0.0f;
			target_vR.x = 0.0f;
			isTurnBehind_ = true;
		}
	}
	// スティック入力が互いに反対(カメラ回転の判定)
	// 上下に入力しているか
	if (target_vL.y * target_vR.y < 0.0f) {
		// 閾値より下なら回転させない
		if (std::abs(sqrtStick.y) <= rotThreshold * 2.0f) {
			target_vL.y = 0.0f;
			target_vR.y = 0.0f;
		}
	}

	// 片方の入力が無かったら0にする[X軸方向]
	if (target_vL.x == 0.0f) {
		target_vR.x = 0.0f;
	}
	if (target_vR.x == 0.0f) {
		target_vL.x = 0.0f;
	}

	// 補間（スムーズな操作）
	float rate = 0.02f;
	if (CheckIsMove(leftStick, rightStick)) {
		rate = 0.1f;
		moveRotMatrix_ = pBB_->GetValue<Actor*>("Player")->GetWorldTF()->GetWorldRotateMatrix();
	}
	vL += (target_vL - vL) * rate;
	vR += (target_vR - vR) * rate;
	const float EPS = 1e-6f;
	if (std::fabs(vR.x) < EPS) {
		vR.x = 0.0f;
	}
	if (std::fabs(vR.y) < EPS) {
		vR.y = 0.0f;
	}
	if (std::fabs(vL.x) < EPS) {
		vL.x = 0.0f;
	}
	if (std::fabs(vL.y) < EPS) {
		vL.y = 0.0f;
	}

	// 差動モデル計算
	Vector2 v = (vR + vL) * 0.5f;

	// 角度代入
	Quaternion q = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, -(target_vR.y - target_vL.y) / treadWidth * maxOmega * deltaTime);
	rot_ = q;

	// Z軸の回転をなくした自機の角度
	Quaternion moveRotZLock = FLMath::LookRotationZLock(Vector3{ 0,0,1 } *moveRotMatrix_);
	// 速度を算出
	vel_ = Vector3{ 0,0,1 } *Matrix4x4::CreateRotateXYZMatrix(moveRotZLock) * v.y;
	vel_ += Vector3{ 1,0,0 } *Matrix4x4::CreateRotateXYZMatrix(moveRotZLock) * v.x;
	vel_.y = 0.0f;
}

void Move::FPSTypeMove() {
	Vector2 lStick = VirtualController::GetInstance()->GetLAxis();
	Vector2 rStick = VirtualController::GetInstance()->GetRAxis();

	// 角度算出
	Quaternion qYaw = Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, rStick.x * maxOmega);
	rot_ = qYaw;

	// 角度から向かう方向算出
	Vector3 vel = Vector3{ 0,0,lStick.y } *Matrix4x4::CreateRotateXYZMatrix(pBB_->GetValue<Actor*>("Player")->GetWorldTF()->rotation * qYaw);
	Vector3 vel2 = Vector3{ lStick.x,0,0 } *Matrix4x4::CreateRotateXYZMatrix(pBB_->GetValue<Actor*>("Player")->GetWorldTF()->rotation * qYaw);
	vel_ = vel + vel2 * maxSpeed;
}

void Move::CheckMoveType() {
	isMoveTypeChange_ = VirtualController::GetInstance()->IsSwitchCommand();

	if (isMoveTypeChange_ && !isPreMoveTypeChange_) {
		if (moveType_ == MoveType::kTank) moveType_ = MoveType::kFPS;
		else if (moveType_ == MoveType::kFPS) moveType_ = MoveType::kTank;
	}

	switch (moveType_) {
	case MoveType::kTank:
		// スティック入力の補正
		Vector2 lStick = AdjustmentStick(VirtualController::GetInstance()->GetLAxis());
		Vector2 rStick = AdjustmentStick(VirtualController::GetInstance()->GetRAxis());

		// 戦車挙動
		DifferentialUpdate(lStick, rStick, stopController_->GetDeltaTime());
		break;
	case MoveType::kFPS:
		FPSTypeMove();
		break;
	}
}

LWP::Math::Vector2 Move::AdjustmentStick(LWP::Math::Vector2 stick) {
	Vector2 result = { 0.0f,0.0f };
	if (std::fabsf(stick.y) >= inputDetectionRange.y) {
		if (std::signbit(stick.y)) { result.y = -1.0f; }
		else { result.y = 1.0f; }
	}

	// 前後入力がない
	if (result.y == 0.0f) {
		if (std::fabsf(stick.x) >= inputDetectionRange.x) {
			result.x = stick.x;
		}
	}
	// 前後移動があるとき
	else {
		if (std::fabsf(stick.x) >= 1.0f - inputDetectionRange.x) {
			result.x = stick.x;
		}
	}

	return result;
}

bool Move::CheckIsMove(Vector2 leftStick, Vector2 rightStick) {
	// 横移動が成立しているか
	if (CheckIsSideMove(leftStick.x, rightStick.x)) { return true; }
	// 前後の移動が成立しているか
	if (CheckIsVerticalMove(leftStick.y, rightStick.y)) { return true; }

	return false;
}

bool Move::CheckIsSideMove(float leftStickX, float rightStickX) {
	if (leftStickX * rightStickX > 0.0f) { return true; }
	return false;
}

bool Move::CheckIsVerticalMove(float leftStickY, float rightStickY) {
	if (leftStickY * rightStickY > 0.0f) { return true; }
	return false;
}

void Move::BodyInclination() {
	// スティック入力の補正
	Vector2 lStick = AdjustmentStick(VirtualController::GetInstance()->GetLAxis());
	Vector2 rStick = AdjustmentStick(VirtualController::GetInstance()->GetRAxis());
	if (!CheckIsSideMove(lStick.x, rStick.x)) {
		effectRot_ = Interpolation::SlerpQuaternion(effectRot_, Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, 0.0f), 0.1f);
		const float EPS = 1e-6f;
		if (std::fabs(effectRot_.x) < EPS) {
			effectRot_.x = 0.0f;
		}
		if (std::fabs(effectRot_.y) < EPS) {
			effectRot_.y = 0.0f;
		}
		if (std::fabs(effectRot_.z) < EPS) {
			effectRot_.z = 0.0f;
		}
		return;
	}

	// Z軸の回転をなくした自機の角度
	Quaternion moveRotZLock = {0,0,0,1};
	Quaternion twist = {0,0,0,1};
	DecomposeSwingTwist(FLMath::LookRotationZLock(Vector3{ 0,0,1 } *moveRotMatrix_), Vector3{ 1,0,0 }, moveRotZLock, twist);
	// 自機の方向ベクトル
	Vector3 playerDir = Vector3{ 0,0,1 } * Matrix4x4::CreateRotateXYZMatrix(moveRotZLock);
	playerDir.y = 0.0f;
	Vector3 moveDir = Vector3{ lStick.x,0,0 } *Matrix4x4::CreateRotateXYZMatrix(moveRotZLock);

	// 現在の速度ベクトルとの角度算出
	float dot = Vector3::Dot(playerDir.Normalize(), moveDir.Normalize());
	dot = 1.0f - dot;
	dot = std::clamp<float>(dot, -1.0f, 1.0f);

	Vector3 cross = Vector3::Cross(playerDir, moveDir);
	float sinTheta = -cross.y; // Y軸回転だけ見る
	sinTheta = std::clamp(sinTheta, -1.0f, 1.0f);

	// 差動モデルx方向の速度
	float velX = (rStick.x + lStick.x) * 0.5f * 0.5f;
	float inclination = std::abs((velX / maxSpeed) * maxInclination);
	// ラジアン算出 [-maxInclination ~ maxInclination]
	float radian = sinTheta * dot * (inclination);
	effectRot_ = Interpolation::SlerpQuaternion(effectRot_, Quaternion::CreateFromAxisAngle(Vector3{ 0,0,1 }, radian), 0.1f) * preEffectRot_.Inverse();
}
