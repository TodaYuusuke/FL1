#include "Player.h"
#include <algorithm>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object;

Player::Player(Camera* camera) {
	camera;
	// 初期位置の設定
	tag_ = "PlayerTag";
	name_ = "Player";
	attackPower_ = 10;

	model_.LoadCube();

	moveController_ = std::make_unique<MoveController>();
	weaponController_ = std::make_unique<WeaponController>();
	// デバッグ用の武器の持ち主を設定
	weaponController_->SetDebugWeaponOwner(this);
}

Player::~Player() {

}

void Player::Init() {
	moveController_->Init();
	weaponController_->Init();
}

void Player::Update() {
	// 移動系統の行動
	moveController_->Update();
	// 武器由来の行動
	weaponController_->Update();

	model_.worldTF.translation += moveController_->GetVel();
	model_.worldTF.rotation = moveController_->GetRot();
}

void Player::DrawGui() {
	if (ImGui::TreeNode("Player")) {
		moveController_->DebugGui();
		weaponController_->DebugGui();
		ImGui::TreePop();
	}
}
