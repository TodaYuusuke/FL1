#include "Player.h"
#include "../Collision/CollisionMask.h"
#include "../Weapon/WeaponManager.h"
#include <algorithm>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object;

Player::Player(FollowCamera* camera, const LWP::Math::Vector3& centerPos) {
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
		OnCollision(hitTarget);
		};
	bodyAABB_.min = { -3.0f, -0.5f, -1.0f };
	bodyAABB_.max = { 3.0f, 9.5f, 1.0f };

	// 偏差射撃機能
	leadingSystem_ = std::make_unique<LeadingSystem>(camera->GetCamera(), blackBoard_);

	// 移動系統の管理
	moveController_ = std::make_unique<MoveController>(this);
	// 武器系統の管理
	weaponController_ = std::make_unique<WeaponController>(leadingSystem_.get(), this);
	weaponController_->SetCenterDist(centerPos_);
	// デバッグ用の武器の持ち主を設定
	weaponController_->SetDebugWeaponOwner(this);

	// jsonファイルの読み込みor作成
	json_.Init("Player.json")
		// 武器
		.BeginGroup("Weapon")
		// 手
		.BeginGroup("Hand")
		.BeginGroup("Left")
		.AddValue<int>("Type", &weaponTypies_[(int)WeaponSide::kLeft])
		.EndGroup()
		.BeginGroup("Right")
		.AddValue<int>("Type", &weaponTypies_[(int)WeaponSide::kRight])
		.EndGroup()
		.EndGroup()
		// 肩
		.BeginGroup("Shoulder")
		.BeginGroup("Left")
		.AddValue<int>("Type", &weaponTypies_[(int)WeaponSide::kLeftShoulder])
		.EndGroup()
		.BeginGroup("Right")
		.AddValue<int>("Type", &weaponTypies_[(int)WeaponSide::kRightShoulder])
		.EndGroup()
		.EndGroup()
		.EndGroup()
		// 当たり判定
		.BeginGroup("Collider")
		.AddValue<float>("Max", &bodyAABB_.max.x)
		.AddValue<float>("Min", &bodyAABB_.min.x)
		.EndGroup()
		.CheckJsonFile();

	// 初期武器を設定
	void (Player:: * setWeapon[(int)WeaponSide::kCount])(IWeapon*) = {
		&Player::SetLeftWeapon,
		&Player::SetRightWeapon,
		&Player::SetLeftShoulderWeapon,
		&Player::SetRightShoulderWeapon
	};
	for (int i = 0; i < weaponTypies_.size(); i++) {
		// 武器なしは含めない
		if (weaponTypies_[i] >= (int)WeaponType::kCount) { continue; }

		// 持たせる武器を作成
		IWeapon* weapon = WeaponManager::GetInstance()->CreateWeapon(weaponTypies_[i], weaponRarities_[i]);
		// 武器の付与
		(this->*setWeapon[i])(weapon);
	}
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

		// 武器
		if (ImGui::TreeNode("Default Weapon")) {
			// 調整項目
			if (ImGui::Button("Save")) {
				json_.Save();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load")) {
				json_.Load();
			}
			weaponTypePreview_ = WeaponManager::GetInstance()->GetWeaponTypePreview();
			weaponTypePreview_.push_back("None");
			weaponRarityPreview_ = WeaponManager::GetInstance()->GetWeaponRarityPreview();
			// 手武器を選択
			if (ImGui::TreeNode("Select hand weapon")) {
				SelectWeaponType(weaponTypies_[(int)WeaponSide::kLeft], "Left");
				SelectWeaponRarity(weaponRarities_[(int)WeaponSide::kLeft], "Left");

				SelectWeaponType(weaponTypies_[(int)WeaponSide::kRight], "Right");
				SelectWeaponRarity(weaponRarities_[(int)WeaponSide::kRight], "Right");

				ImGui::TreePop();
			}
			// 肩武器を選択
			if (ImGui::TreeNode("Select shoulder weapon")) {
				SelectWeaponType(weaponTypies_[(int)WeaponSide::kLeftShoulder], "Left");
				SelectWeaponRarity(weaponRarities_[(int)WeaponSide::kLeftShoulder], "Left");

				SelectWeaponType(weaponTypies_[(int)WeaponSide::kRightShoulder], "Right");
				SelectWeaponRarity(weaponRarities_[(int)WeaponSide::kRightShoulder], "Right");

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		// 当たり判定
		if (ImGui::TreeNode("HP")) {
			hp_->DebugGui();
			ImGui::TreePop();
		}
		// 当たり判定
		if (ImGui::TreeNode("Collider")) {
			// 調整項目
			if (ImGui::Button("Save")) {
				json_.Save();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load")) {
				json_.Load();
			}
			ImGui::DragFloat3("Min", &bodyAABB_.min.x, 0.01f);
			ImGui::DragFloat3("Max", &bodyAABB_.max.x, 0.01f);
			ImGui::TreePop();
		}
		// 武器
		weaponController_->DebugGui();

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
		// 機能
		if (ImGui::TreeNode("Systems")) {
			leadingSystem_->DebugGui();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void Player::SelectWeaponType(int& selectedWeaponType, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponTypePreview_.empty()) {
		const char* combo_preview_value = weaponTypePreview_[selectedWeaponType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < weaponTypePreview_.size(); n++) {
				const bool is_selected = ((int)selectedWeaponType == n);
				std::string selectableLabel = weaponTypePreview_[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedWeaponType = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found weapon"));
	}
}

void Player::SelectWeaponRarity(int& selectedWeaponRarity, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponRarityPreview_.empty()) {
		const char* combo_preview_value = weaponRarityPreview_[selectedWeaponRarity].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < weaponRarityPreview_.size(); n++) {
				const bool is_selected = ((int)selectedWeaponRarity == n);
				std::string selectableLabel = weaponRarityPreview_[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedWeaponRarity = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found weapon"));
	}
}

LWP::Math::Vector3 Player::GetCenterPosition() {
	return model_.worldTF.GetWorldPosition() + centerPos_ * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
}
