#include "Move.h"
#include "../../../../../Componets/Input/VirtualController.h"
#include "../../../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../../World/World.h"
#include "../../../../../Componets/InputMyController/ControllerReceiver.h"
#include <numbers>

using namespace FLMath;
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
		// 旋回の閾値
		.AddValue<float>("RotThreshold", &rotThreshold)
		// 180度の閾値
		.AddValue<float>("TurnThreshold", &turnThreshold)
		// 180度旋回にかかる時間
		.AddValue<float>("TurnTime", &turnTime)
		// 入力を検出する範囲
		.AddValue<Vector2>("InputDetectionRange", &inputDetectionRange)
		.CheckJsonFile();
}

void Move::Init() {
	turnRadian_ = {};
	preTurnRadian_ = {};
}

void Move::Update() {
	// 180度旋回入力があるか
	//isTurnBehind_ = VirtualController::GetInstance()->GetPress(BindActionType::kTurn);
	isTurnBehind_ = false;
	// 移動方式選択
	CheckMoveType();

	// 180度回転入力があった時の処理
	TurnBehind();
	turnTime_.Update();

	// タイムスケール適用
	vel_ *= stopController_->GetDeltaTime();

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
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

LWP::Math::Vector2 Move::AdjustmentStick(LWP::Math::Vector2 stick) {
	Vector2 result = { 0.0f,0.0f };
	if (std::fabsf(stick.x) >= inputDetectionRange.x) {
		if (std::signbit(stick.x)) { result.x = -1.0f; }
		else { result.x = 1.0f; }
	}
	result.x = stick.x;
	if (std::fabsf(stick.y) >= inputDetectionRange.y) {
		if (std::signbit(stick.y)) { result.y = -1.0f; }
		else { result.y = 1.0f; }
	}

	return result;
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
	}

	if (!turnTime_.GetIsActive()) { return; }

	turnRadian_ = LWP::Utility::Interp::Lerp(Vector3{}, Vector3{ 0.0f, (float)std::numbers::pi, 0.0f }, turnTime_.GetProgress());

	// 前回の角度を引いて角速度を求める
	Vector3 radian = turnRadian_ - preTurnRadian_;
	rot_ *= Quaternion::CreateFromAxisAngle({ 0.0f,1.0f,0.0f }, radian.y);
}

void Move::DifferentialUpdate(LWP::Math::Vector2 leftStick, LWP::Math::Vector2 rightStick, float deltaTime) {
	// 履帯の目標速度（入力を反転したい場合は符号調整）
	float target_vL = leftStick.y * maxSpeed;
	float target_vR = rightStick.y * maxSpeed;

	// 片方の入力が無かったら0にする
	//if (target_vL == 0.0f) {
	//	target_vR = 0.0f;
	//}
	//if (target_vR == 0.0f) {
	//	target_vL = 0.0f;
	//}
	//Vector2 diff = {
	//	leftStick.x - rightStick.x,
	//	leftStick.y - rightStick.y
	//};
	//diff.x = std::sqrtf(diff.x * diff.x);
	//diff.y = std::sqrtf(diff.y * diff.y);

	// 値の修正される前のスティックの入力値で比較
	Vector2 lStick = VirtualController::GetInstance()->GetLAxis();
	Vector2 rStick = VirtualController::GetInstance()->GetRAxis();
	float sqrtStick = (lStick.y - rStick.y);
	// スティック入力が互いに反対
	if (target_vL * target_vR < 0.0f) {
		if (std::sqrtf(sqrtStick * sqrtStick) >= turnThreshold * 2.0f) {
			target_vL = 0.0f;
			target_vR = 0.0f;
			isTurnBehind_ = true;
		}
	}

	// 補間（スムーズな操作）
	vL += (target_vL - vL) * 0.1f;
	vR += (target_vR - vR) * 0.1f;

	// 差動モデル計算
	float v = (vR + vL) * 0.5f;
	omega = (vR - vL) / treadWidth * maxOmega;
	// 更新
	angle += -omega * deltaTime;

	// 回転角が2πを超えたら初期化
	if (angle <= -(float)std::numbers::pi * 2.0f) {
		angle = std::fmodf(std::fabsf(angle), -(float)std::numbers::pi * 2.0f);
	}
	if (angle >= (float)std::numbers::pi * 2.0f) {
		angle = std::fmodf(std::fabsf(angle), (float)std::numbers::pi * 2.0f);
	}

	// 角度[ラジアン]
	Vector3 rot = {
		0.0f,
		angle,
		0.0f,
	};
	// 角度代入
	Quaternion q = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, -(target_vR - target_vL) / treadWidth * maxOmega);
	rot_ = q;

	// 速度を算出
	vel_ = Vector3{ 0,0,1 } *LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pBB_->GetValue<Actor*>("Player")->GetWorldTF()->rotation) * v;
	// 横移動をしているなら算出
	if (leftStick.x * rightStick.x > 0.0f) {
		Vector3 sideMove = { leftStick.x * maxSpeed, 0.0f, 0.0f };
		vel_ += sideMove * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(pBB_->GetValue<Actor*>("Player")->GetWorldTF()->rotation);
	}
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