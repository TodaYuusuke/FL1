#include "Player.h"
#include "../Collision/CollisionMask.h"
#include <algorithm>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object;

Player::Player(Camera* camera) {
	// 初期位置の設定
	tag_ = "Player";
	name_ = "Player";
	attackPower_ = 10;

	// 黒板生成
	blackBoard_ = new BlackBoard();
	blackBoard_->SetValue<Actor*>("Player", this);

	// モデル生成
	model_.LoadShortPath("Player/Player.gltf");

	// 体の判定生成
	bodyCollision_.SetFollow(&model_.worldTF);
	bodyCollision_.isActive = true;
	bodyCollision_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	// 自機の所属しているマスクを設定
	bodyCollision_.mask.SetBelongFrag(GameMask::player);
	// 当たり判定をとる対象のマスクを設定
	bodyCollision_.mask.SetHitFrag(GameMask::attack);
	bodyCollision_.enterLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		};

	// 偏差射撃機能
	leadingSystem_ = std::make_unique<LeadingSystem>(camera, blackBoard_);

	// 移動系統の管理
	moveController_ = std::make_unique<MoveController>();
	// 武器系統の管理
	weaponController_ = std::make_unique<WeaponController>(leadingSystem_.get());
	// デバッグ用の武器の持ち主を設定
	weaponController_->SetDebugWeaponOwner(this);
}

Player::~Player() {
	delete blackBoard_;
}

void Player::Init() {
	leadingSystem_->Init();
	moveController_->Init();
	weaponController_->Init();
}

void Player::Update() {
	leadingSystem_->Update();

	// 移動系統の行動
	moveController_->Update();
	// 武器由来の行動
	weaponController_->Update();

	model_.worldTF.translation += moveController_->GetVel();
	model_.worldTF.rotation = moveController_->GetRot();
}

void Player::DrawGui() {
	if (ImGui::TreeNode("Player")) {
		// モデル
		if (ImGui::TreeNode("Model")) {
			model_.DebugGUI();
			ImGui::TreePop();
		}

		// 行動
		if (ImGui::TreeNode("Actions")) {
			moveController_->DebugGui();
			ImGui::TreePop();
		}

		// 武器
		weaponController_->DebugGui();

		// 機能
		if (ImGui::TreeNode("Systems")) {
			leadingSystem_->DebugGui();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
