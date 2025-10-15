#include "Move.h"
#include <numbers>

using namespace FLMath;
using namespace LWP::Math;
using namespace LWP::Input;

Move::Move() {
	stateName_ = "Move";
}

void Move::Init() {

}

void Move::Update() {
	Vector2 lStick = AdjustmentStick(LWP::Input::Controller::GetLStick());
	Vector2 rStick = AdjustmentStick(LWP::Input::Controller::GetRStick());
	//Vector2 lStick = LWP::Input::Controller::GetLStick();
	//Vector2 rStick = LWP::Input::Controller::GetRStick();
	//StickDiff(lStick, rStick);

	// 戦車挙動
	DifferentialUpdate(lStick, rStick, 1.0f);
}

void Move::DebugGui() {
	if (ImGui::TreeNode("Move")) {
		ImGui::DragFloat("MaxSpeed", &maxSpeed, 0.01f, -1000.0f, 1000.0f);
		ImGui::DragFloat("MaxOmega", &maxOmega, 0.01f, -1000.0f, 1000.0f);
		ImGui::DragFloat("TreadWidth", &treadWidth, 0.01f, -1000.0f, 1000.0f);
		ImGui::DragFloat("Omega", &omega);
		ImGui::DragFloat("Angle", &angle);
		ImGui::TreePop();
	}
}

LWP::Math::Vector2 Move::AdjustmentStick(LWP::Math::Vector2 stick) {
	Vector2 result = { 0.0f,0.0f };
	if (std::fabsf(stick.x) >= 0.8f) {
		if (std::signbit(stick.x)) { result.x = -1.0f; }
		else { result.x = 1.0f; }
	}
	result.x = stick.x;
	if (std::fabsf(stick.y) >= 0.01f) {
		if (std::signbit(stick.y)) { result.y = -1.0f; }
		else { result.y = 1.0f; }
	}

	return result;
}

void Move::DifferentialUpdate(LWP::Math::Vector2 leftStick, LWP::Math::Vector2 rightStick, float deltaTime) {
	// 履帯の目標速度（入力を反転したい場合は符号調整）
	float target_vL = leftStick.y * maxSpeed;
	float target_vR = rightStick.y * maxSpeed;

	// 片方の入力が無かったら0にする
	if (target_vL == 0.0f) {
		target_vR = 0.0f;
	}
	if (target_vR == 0.0f) {
		target_vL = 0.0f;
	}
	Vector2 diff = {
		leftStick.x - rightStick.x,
		leftStick.y - rightStick.y
	};
	diff.x = std::sqrtf(diff.x * diff.x);
	diff.y = std::sqrtf(diff.y * diff.y);

	// スティック入力が互いに反対
	if (target_vL * target_vR < 0.0f) {
		// 値の修正される前のスティックの入力値で比較
		float sqrtStick = (Controller::GetLStick().y - Controller::GetRStick().y);
		if (std::sqrtf(sqrtStick * sqrtStick) <= 1.7f) {
			target_vL = 0.0f;
			target_vR = 0.0f;
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
	rot_ = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, rot.x);
	rot_ = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, rot.y) * rot_;

	// 速度を算出
	vel_ = Vector3{ 0,0,1 } *LWP::Math::Matrix4x4::CreateRotateXYZMatrix(rot) * v;
	// 横移動をしているなら算出
	if (leftStick.x * rightStick.x > 0.0f) {
		Vector3 sideMove = { leftStick.x * maxSpeed, 0.0f, 0.0f };
		vel_ += sideMove * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(rot);
	}
}

//void Move::StickDiff(LWP::Math::Vector2& leftStick, LWP::Math::Vector2& rightStick) {
	//Vector2 diff = {
	//	leftStick.x - rightStick.x,
	//	leftStick.y - rightStick.y
	//};
	//diff.x *= diff.x;
	//diff.x = std::sqrtf(diff.x);
	//diff.y *= diff.y;
	//diff.y = std::sqrtf(diff.y);

	//if (diff.x >= 0.2f) {
	//	leftStick.x = 0.0f;
	//	rightStick.x = 0.0f;
	//}
	//else {
	//	leftStick.x =(leftStick.x + rightStick.x) * 0.5f;
	//	rightStick.x =(leftStick.x + rightStick.x) * 0.5f;
	//}

	//// スティック入力が互いに反対
	//if (leftStick.y * rightStick.y < 0.0f) {
	//	float sqrtStick = (Controller::GetLStick().y - Controller::GetRStick().y);
	//	if (std::sqrtf(sqrtStick * sqrtStick) <= 1.5f) {
	//		leftStick.y = 0.0f;
	//		rightStick.y = 0.0f;
	//	}
	//}
	//else if (diff.y >= 0.4f) {
	//	leftStick.y = 0.0f;
	//	rightStick.y = 0.0f;
	//}
	//else {
	//	leftStick.y = (leftStick.y + rightStick.y) * 0.5f;
	//	rightStick.y = (leftStick.y + rightStick.y) * 0.5f;
	//}

	//leftStick = {
	//	(leftStick.x + rightStick.x) * 0.5f,
	//	(leftStick.y + rightStick.y) * 0.5f
	//};
	//rightStick = {
	//(leftStick.x + rightStick.x) * 0.5f,
	//(leftStick.y + rightStick.y) * 0.5f
	//};
//}
