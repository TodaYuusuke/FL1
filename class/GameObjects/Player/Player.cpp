#include "Player.h"
#include <algorithm>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object;

Player::Player(Camera* camera) {
	// 初期位置の設定
	pos = { 0.0f, 0.0f };
	tag = "PlayerTag";
	name = "Player";
	attackPower = 10;

	model_.LoadCube();
}

Player::~Player() {

}

void Player::Update(float delta_time) {
	// 
	DifferentialUpdate(LWP::Input::Controller::GetLStick().y, LWP::Input::Controller::GetRStick().y, delta_time);
}

void Player::Draw() const {
	//obj_->Draw();
	//DrawCircle(mPosition.int_x(), mPosition.int_y(), 32, GetColor(0, 255, 0), TRUE);
}

void Player::DrawGui() {
	if (ImGui::TreeNode("Player")) {
		ImGui::DragFloat("MaxSpeed", &maxSpeed, 0.01f, -1000.0f, 1000.0f);
		ImGui::DragFloat("TreadWidth", &treadWidth, 0.01f, -1000.0f, 1000.0f);
		ImGui::DragFloat("Omega", &omega);
		ImGui::DragFloat("Angle", &angle);
		ImGui::TreePop();
	}
}

void Player::InputHandle() {

}

void Player::DifferentialUpdate(float leftStickY, float rightStickY, float deltaTime) {
	// 履帯の目標速度（入力を反転したい場合は符号調整）
	float target_vL = leftStickY * maxSpeed;
	float target_vR = rightStickY * maxSpeed;

	// 補間（スムーズな操作）
	vL += (target_vL - vL) * 0.1f;
	vR += (target_vR - vR) * 0.1f;

	// 差動モデル計算
	float v = (vR + vL) * 0.5f;
	omega = (vR - vL) / treadWidth;

	// 更新
	angle += omega * deltaTime;
	Vector3 vel = {
		v * cosf(angle) * deltaTime,
		0.0f,
		v * sinf(angle) * deltaTime
	};

	model_.worldTF.translation += vel;

	// 回転
	Vector3 radian = {
		atan2f(-vel.y, sqrtf(vel.x * vel.x + vel.z * vel.z)),
		atan2f(vel.x, vel.z),
		0.0f
	};
	model_.worldTF.rotation = Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, radian.x);
	model_.worldTF.rotation = Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, radian.y) * model_.worldTF.rotation;
}
