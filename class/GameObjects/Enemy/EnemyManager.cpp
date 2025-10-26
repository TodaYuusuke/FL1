#include "EnemyManager.h"
#include "Melee/MeleeAttacker.h"
#include "Gunner/Gunner.h"
#include "Drone/Drone.h"
#include "Test/TestEnemy.h"
#include "../Weapon/Gun/MachineGun/MachineGun.h"
#include "../Weapon/WeaponManager.h"
#include "EnemyConfig.h"

using namespace EnemyConfig;

EnemyManager::EnemyManager(IWorld* world) {
	pWorld_ = world;

	// デバッグ用の情報を生成
	CreateDebugData();

#pragma region コピー元となる敵生成
	for (int i = 0; i < EnemyConfig::Name::name.size(); i++) {
		selectCreateEnemyType_ = i;
		sampleEnemies_[i] = CreateEnemy();
		// 調整データ作成
		sampleEnemies_[i]->CreateJsonData();
	}
#pragma endregion
}

EnemyManager::~EnemyManager() {
	for (Actor* actor : enemies_) {
		delete actor;
	}
	// 調整されたオリジナル
	for (auto it = sampleEnemies_.begin(); it != sampleEnemies_.end(); it++) {
		delete it->second;
	}
	enemies_.clear();
}

void EnemyManager::Init() {
	for (Actor* actor : enemies_) {
		actor->Init();
	}
}

void EnemyManager::Update() {
	for (Actor* actor : enemies_) {
		// 前回の座標
		actor->SetPreTranslation(actor->GetWorldTF()->GetWorldPosition());

		actor->Update();
	}
	// 削除
	enemies_.erase(
		std::remove_if(enemies_.begin(), enemies_.end(),
			[](Actor* actor) {
				if (!actor->GetIsAlive()) {
					// 武器を落とす
					WeaponManager::GetInstance()->DropWeapon(actor->GetWeapon());

					// 敵の解放
					delete actor;
					actor = nullptr;
					return true; // vectorから削除対象
				}
				return false;
			}),
		enemies_.end()
	);
}

void EnemyManager::DebugGui() {
	if (ImGui::BeginTabItem("Enemy")) {
		// 各敵のbehaviorTreeの調整
		if (ImGui::TreeNode("Behavior-Tree edit")) {
			SwitchNodeEditorCanvas(btEditor_->GetEditorContext());
			SelectJsonFile();
			btEditor_->Update();
			btEditor_->Draw();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Current enemy BT")) {
			for (Actor* actor : enemies_) {
				SwitchNodeEditorCanvas(actor->GetEditorContext());
				actor->DrawGui();
			}
			ImGui::TreePop();
		}

		// 敵作成
		if (ImGui::TreeNode("Create")) {
			// 作成する敵を選択
			SelectCreateEnemy();
			// 生成座標
			ImGui::DragFloat3("CreateTranslation", &createPos_.x, 0.01f);
			// テスト敵のみ調整可能
			if (selectCreateEnemyType_ == (int)EnemyType::kTest) {
				// 速度
				ImGui::DragFloat3("Velocity", &createVel_.x, 0.01f);
			}

			// 敵生成
			if (ImGui::Button("Create")) {
				// プレビューで選択した敵を生成
				enemies_.push_back(CreateEnemy());
			}
			ImGui::TreePop();
		}

		ImGui::EndTabItem();
	}
}

Actor* EnemyManager::CreateMeleeEnemy() {
	// 近接敵
	MeleeAttacker* actor = new MeleeAttacker(pWorld_, createID_, enemyBTFileNamePreview_[(int)EnemyType::kMelee]);
	actor->SetTranslation(createPos_);

	// 武器を付与
	GiveWeapon(actor);
	createID_++;

	return actor;
}

Actor* EnemyManager::CreateGunnerEnemy() {
	// 遠距離敵
	Gunner* actor = new Gunner(pWorld_, createID_, enemyBTFileNamePreview_[(int)EnemyType::kGunner]);
	actor->SetTranslation(createPos_);

	// 武器を付与
	GiveWeapon(actor);
	createID_++;

	return actor;
}

Actor* EnemyManager::CreateDroneEnemy() {
	// ドローン敵
	Drone* actor = new Drone(pWorld_, createID_, enemyBTFileNamePreview_[(int)EnemyType::kDrone]);
	actor->SetTranslation(createPos_);
	actor->SetFloatHeight(dronefloatHeight_);

	// 武器を付与
	GiveWeapon(actor);
	createID_++;

	return actor;
}

void EnemyManager::GiveWeapon(Actor* actor) {
	// 持たせる武器を作成
	IWeapon* weapon = WeaponManager::GetInstance()->CreateWeapon(0, 0);
	// 武器の付与
	WeaponManager::GetInstance()->PickUpWeapon(weapon, actor);
}

Actor* EnemyManager::CreateTestEnemy() {
	// テスト敵
	TestEnemy* actor = new TestEnemy();
	actor->SetTranslation(createPos_);
	actor->SetVelocity(createVel_);

	return actor;
}

Actor* EnemyManager::CreateEnemy() {
	switch (selectCreateEnemyType_) {
		// 近接
	case (int)EnemyType::kMelee:
		return CreateMeleeEnemy();
		break;
		// 遠距離
	case (int)EnemyType::kGunner:
		return CreateGunnerEnemy();
		break;
	case (int)EnemyType::kDrone:
		return CreateDroneEnemy();
		break;
		// テスト
	case (int)EnemyType::kTest:
		return CreateTestEnemy();
		break;
	default:
		return nullptr;
		break;
	}
}

// ------------ デバッグ用関数↓------------ //

void EnemyManager::SelectCreateEnemy() {
	ImGui::Text("Select create enemy");
	// 追加する敵のプレビュー作成
	if (!enemyTypePreview_.empty()) {
		const char* combo_preview_value = enemyTypePreview_[selectCreateEnemyType_].c_str();
		if (ImGui::BeginCombo((" "), combo_preview_value)) {
			for (int n = 0; n < enemyTypePreview_.size(); n++) {
				const bool is_selected = ((int)selectCreateEnemyType_ == n);
				if (ImGui::Selectable(enemyTypePreview_[n].c_str(), is_selected)) {
					selectCreateEnemyType_ = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found add enemy"));
	}
}

void EnemyManager::SelectJsonFile() {
	ImGui::Text("Select Behavior-Tree file");
	// 読み込むbehaviorTreeのプレビュー作成
	if (!enemyBTFileNamePreview_.empty()) {
		const char* combo_preview_value = enemyBTFileNamePreview_[selectBTFileName_].c_str();
		if (ImGui::BeginCombo((" "), combo_preview_value)) {
			for (int n = 0; n < enemyBTFileNamePreview_.size(); n++) {
				const bool is_selected = ((int)selectBTFileName_ == n);
				if (ImGui::Selectable(enemyBTFileNamePreview_[n].c_str(), is_selected)) {
					selectBTFileName_ = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found behavior-tree file"));
	}

	// 読み込み
	if (ImGui::Button("Load")) {
		btEditor_->SelectLoadFile(enemyBTFileNamePreview_[selectBTFileName_]);
	}
}

void EnemyManager::CreateDebugData() {
	// ビヘイビアツリーの編集クラス
	btEditor_ = std::make_unique<BehaviorTreeGraph>(true);

#pragma region プレビュー
	// 武器種生成プレビュー
	for (int i = 0; i < EnemyConfig::Name::name.size(); i++) {
		enemyTypePreview_.push_back(EnemyConfig::Name::name[i]);
	}
	// 武器レアリティ生成プレビュー
	for (int i = 0; i < EnemyConfig::BTFileName::fileName.size(); i++) {
		enemyBTFileNamePreview_.push_back(EnemyConfig::BTFileName::fileName[i]);
	}
#pragma endregion
}
