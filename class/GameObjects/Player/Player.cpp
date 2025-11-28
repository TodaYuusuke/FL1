#include "Player.h"
#include "../Collision/CollisionMask.h"
#include <algorithm>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object;

Player::Player(Camera* camera, const LWP::Math::Vector3& centerPos) {
	// 初期位置の設定
	tag_ = "Player";
	name_ = "Player";
	attackPower_ = 10;
	centerPos_ = centerPos;

	// HP
	hp_ = std::make_unique<Health>(100.0f);

	// 黒板生成
	blackBoard_ = new BlackBoard();
	blackBoard_->SetValue<Actor*>("Player", this);

	// モデル生成
	model_.LoadShortPath("Player/Player.gltf");
	model_.Update();

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
	bodyAABB_.min = { -3.0f, -0.5f, -1.0f };
	bodyAABB_.max = { 3.0f, 9.5f, 1.0f };

	// 偏差射撃機能
	leadingSystem_ = std::make_unique<LeadingSystem>(camera, blackBoard_);

	// 移動系統の管理
	moveController_ = std::make_unique<MoveController>(this);
	// 武器系統の管理
	weaponController_ = std::make_unique<WeaponController>(leadingSystem_.get(), this);
	weaponController_->SetCenterDist(centerPos_);
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
	// HP
	hp_->Update();

	// 偏差射撃
	leadingSystem_->Update();

	// 移動系統の行動
	moveController_->Update();
	// 武器由来の行動
	weaponController_->Update();

	weaponVel_.x = std::clamp<float>(weaponVel_.x, -5.0f, 5.0f);
	weaponVel_.y = std::clamp<float>(weaponVel_.y, -5.0f, 5.0f);
	weaponVel_.z = std::clamp<float>(weaponVel_.z, -5.0f, 5.0f);

	model_.worldTF.translation += moveController_->GetVel() + weaponVel_;
	model_.worldTF.rotation = moveController_->GetRot();

	weaponVel_ = { 0.0f,0.0f,0.0f };
}

void Player::DrawGui() {
	if (ImGui::TreeNode("Player")) {
		// モデル
		if (ImGui::TreeNode("Model")) {
			model_.DebugGUI();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Collider")) {
			ImGui::DragFloat3("Min", &bodyAABB_.min.x, 0.01f);
			ImGui::DragFloat3("Max", &bodyAABB_.max.x, 0.01f);
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

LWP::Math::Vector3 Player::GetCenterPosition() {
	return model_.worldTF.GetWorldPosition() + centerPos_ * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
}
