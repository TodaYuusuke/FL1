#include "Move.h"
#include <numbers>

using namespace FLMath;
using namespace LWP::Math;

Move::Move() {
	stateName_ = "Move";
}

void Move::Init() {

}

void Move::Update() {
	Vector2 lStick = AdjustmentStick(LWP::Input::Controller::GetLStick());
	Vector2 rStick = AdjustmentStick(LWP::Input::Controller::GetRStick());

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
		if(std::signbit(stick.x)){ result.x = -1.0f; }
		else{ result.x = 1.0f; }
	}
	if (std::fabsf(stick.y) >= 0.5f) {
		if (std::signbit(stick.y)) { result.y = -1.0f; }
		else { result.y = 1.0f; }
	}

	return result;
}

void Move::DifferentialUpdate(LWP::Math::Vector2 leftStick, LWP::Math::Vector2 rightStick, float deltaTime) {
	// 履帯の目標速度（入力を反転したい場合は符号調整）
	float target_vL = leftStick.y* maxSpeed;
	float target_vR = rightStick.y* maxSpeed;

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