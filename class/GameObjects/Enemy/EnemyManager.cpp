#include "EnemyManager.h"
#include "Melee/MeleeAttacker.h"
#include "Gunner/Gunner.h"
#include "Test/TestEnemy.h"
#include "../Weapon/Gun/MachineGun/MachineGun.h"
#include "EnemyConfig.h"

using namespace EnemyConfig;

EnemyManager::EnemyManager(IWorld* world) {
	pWorld_ = world;

	// デバッグ用の情報を生成
	CreateDebugData();

#pragma region コピー元となる敵生成
	// 近接敵
	MeleeAttacker* melee = new MeleeAttacker(pWorld_, createID_, "resources/json/BT/BT_Melee.json");
	melee->SetTranslation(createPos_);
	sampleEnemies_[kMelee] = melee;
	// 遠距離敵
	Gunner* gunner = new Gunner(pWorld_, createID_, "resources/json/BT/BT_Gunner.json");
	gunner->SetTranslation(createPos_);
	sampleEnemies_[kGunner] = gunner;
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
	for (Actor* actor : enemies_) {
		if (actor->GetIsAlive()) continue;

		delete actor;
		auto newEnd = std::remove(enemies_.begin(), enemies_.end(), actor);
		enemies_.erase(newEnd, enemies_.end());
	}

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
			// 作成する武器を選択
			//SelectCreateWeapon();
			// 生成座標
			ImGui::DragFloat3("CreateTranslation", &createPos_.x, 0.01f);
			// テスト敵のみ調整可能
			if (selectCreateEnemyType_ == kTest) {
				// 速度
				ImGui::DragFloat3("Velocity", &createVel_.x, 0.01f);
			}

			// 敵生成
			if (ImGui::Button("Create")) {
				// プレビューで選択した敵を生成
				CreateEnemy();
			}
			ImGui::TreePop();
		}

		ImGui::EndTabItem();
	}
}

void EnemyManager::CreateMeleeEnemy() {
	// 近接敵
	MeleeAttacker* actor = new MeleeAttacker(pWorld_, createID_, enemyBTFileNamePreview_[kMelee]);
	actor->SetTranslation(createPos_);
	enemies_.push_back(actor);

	createID_++;
}

void EnemyManager::CreateGunnerEnemy() {
	// 遠距離敵
	Gunner* actor = new Gunner(pWorld_, createID_, enemyBTFileNamePreview_[kGunner]);
	actor->SetTranslation(createPos_);

	WeaponData data = {
		"Gun/ShotGun/Rifle.obj",
		0.1f,
		0.0f,
		0.0f,
		60.0f,
		10.0f,
		1.0f,
		1.0f
	};
	MachineGun* gun = new MachineGun(data);
	gun->SetParent(actor);
	gun->SetTranslation(LWP::Math::Vector3{ 1.0f, -0.5f,2.0f });

	actor->ChangeWeapon(gun);

	enemies_.push_back(actor);

	createID_++;
}

void EnemyManager::CreateTestEnemy() {
	// テスト敵
	TestEnemy* actor = new TestEnemy();
	actor->SetTranslation(createPos_);
	actor->SetVelocity(createVel_);
	enemies_.push_back(actor);
}

void EnemyManager::CreateEnemy() {
	switch (selectCreateEnemyType_) {
		// 近接
	case kMelee:
		CreateMeleeEnemy();
		break;
		// 遠距離
	case kGunner:
		CreateGunnerEnemy();
		break;
		// テスト
	case kTest:
		CreateTestEnemy();
		break;
	}
}

void EnemyManager::CreateWeapon() {
	switch (selectCreateEnemyType_) {
		// 近接
	case kMelee:
		CreateMeleeEnemy();
		break;
		// 遠距離
	case kGunner:
		CreateGunnerEnemy();
		break;
		// テスト
	case kTest:
		CreateTestEnemy();
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

void EnemyManager::SelectCreateWeapon() {
	ImGui::Text("Select create weapon");

	for (int i = 0; i < weaponPreview_.size(); i++) {
		ImGui::RadioButton(weaponPreview_[i].c_str(), &selectWeapon_, i);
		i++;
	}

	// 武器生成
	if (ImGui::Button("Create")) {
		// プレビューで選択した敵を生成
		//CreateEnemy();
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
	// 敵生成
	enemyTypePreview_.push_back("Melee");
	enemyTypePreview_.push_back("Gunner");
	enemyTypePreview_.push_back("Test");

	// 武器生成
	weaponPreview_.push_back("MachineGun");
	weaponPreview_.push_back("ShotGun");
	weaponPreview_.push_back("Rifle");
	weaponPreview_.push_back("Launcher");
	weaponPreview_.push_back("Missile");
	weaponPreview_.push_back("Melee");

	// behaviorTreeファイル
	enemyBTFileNamePreview_.push_back("resources/json/BT/BT_Melee.json");
	enemyBTFileNamePreview_.push_back("resources/json/BT/BT_Gunner.json");
#pragma endregion
}
