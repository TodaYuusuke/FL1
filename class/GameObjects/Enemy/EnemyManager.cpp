#include "EnemyManager.h"
#include "Melee/MeleeAttacker.h"
#include "Gunner/Gunner.h"
#include "Drone/Drone.h"
#include "Test/TestEnemy.h"
#include "../Weapon/Gun/MachineGun/MachineGun.h"
#include "../Weapon/WeaponManager.h"
#include "EnemyConfig.h"

using namespace EnemyConfig;
using namespace LWP;
using namespace LWP::Math;

EnemyManager::EnemyManager(IWorld* world) {
	pWorld_ = world;

	// デバッグ用の情報を生成
	CreateDebugData();

#pragma region コピー元となる敵生成
	for (int i = 0; i < EnemyConfig::Name::name.size(); i++) {
		// 調整データ作成
		CreateJsonData(jsonDatas_[i], sampleEnemies_[i], EnemyConfig::Name::name[i]);
	}
#pragma endregion
}

EnemyManager::~EnemyManager() {
	for (Actor* actor : enemies_) {
		delete actor;
	}
	enemies_.clear();
	sampleEnemies_.clear();
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
					for (int i = 0; i < actor->GetWeapon().size(); i++) {
						WeaponManager::GetInstance()->DropWeapon(actor->GetWeapon()[i]);
					}

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
		// jsonによる調整データ
		if (ImGui::TreeNode("Json")) {
			// 作成する敵を選択
			SelectCreateEnemy();

			// 選択した敵を調整
			SelectEnemyDataGui(jsonDatas_[selectCreateEnemyType_], sampleEnemies_[selectCreateEnemyType_]);
			//sampleEnemies_[selectCreateEnemyType_]->DrawGui();

			ImGui::TreePop();
		}

		// 各敵のbehaviorTreeの調整
		if (ImGui::TreeNode("BT edit")) {
			// 読み込むファイルを選択
			SwitchNodeEditorCanvas(btEditor_->GetEditorContext());
			SelectJsonFile();
			// 読み込み
			if (ImGui::Button("Load")) {
				btEditor_->SelectLoadFile(enemyBTFileNamePreview_[selectBTFileName_]);
			}

			btEditor_->Update();
			btEditor_->Draw();
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
	MeleeAttacker* actor = new MeleeAttacker(pWorld_, createID_, sampleEnemies_[(int)EnemyType::kMelee]);
	actor->SetTranslation(createPos_);

	// 武器を付与
	GiveWeapon(actor, sampleEnemies_[(int)EnemyType::kMelee]);
	createID_++;

	return actor;
}

Actor* EnemyManager::CreateGunnerEnemy() {
	// 遠距離敵
	Gunner* actor = new Gunner(pWorld_, createID_, sampleEnemies_[(int)EnemyType::kGunner]);
	actor->SetTranslation(createPos_);

	// 武器を付与
	GiveWeapon(actor, sampleEnemies_[(int)EnemyType::kGunner]);
	createID_++;

	return actor;
}

Actor* EnemyManager::CreateDroneEnemy() {
	// ドローン敵
	Drone* actor = new Drone(pWorld_, createID_, sampleEnemies_[(int)EnemyType::kDrone]);
	actor->SetTranslation(createPos_);
	actor->SetFloatHeight(dronefloatHeight_);

	// 武器を付与
	GiveWeapon(actor, sampleEnemies_[(int)EnemyType::kDrone]);
	createID_++;

	return actor;
}

void EnemyManager::GiveWeapon(Actor* actor, const EnemyData& data) {
	for (int i = 0; i < data.containWeaponTypes.size(); i++) {
		if (data.containWeaponTypes[i] >= (int)WeaponType::kCount) { continue; }

		// 持たせる武器を作成
		IWeapon* weapon = WeaponManager::GetInstance()->CreateWeapon(data.containWeaponTypes[i], 0);
		// 武器の装着位置設定
		SetWeaponPos(weapon, i);
		// 武器の付与
		WeaponManager::GetInstance()->PickUpWeapon(weapon, actor, i);
	}
}

// ------------ デバッグ用関数↓------------ //

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
}

void EnemyManager::SelectWeaponType(int& selectedWeaponType, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponTypePreview_.empty()) {
		const char* combo_preview_value = weaponTypePreview_[selectedWeaponType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < weaponTypePreview_.size(); n++) {
				const bool is_selected = ((int)selectedWeaponType == n);
				std::string label = weaponTypePreview_[n];
				if (ImGui::Selectable(label.c_str(), is_selected)) {
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

void EnemyManager::SelectWeaponRarity() {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponRarityPreview_.empty()) {
		const char* combo_preview_value = weaponRarityPreview_[selectedWeaponRarity_].c_str();
		if (ImGui::BeginCombo(("##Weapon rarity"), combo_preview_value)) {
			for (int n = 0; n < weaponRarityPreview_.size(); n++) {
				const bool is_selected = ((int)selectedWeaponRarity_ == n);
				if (ImGui::Selectable(weaponRarityPreview_[n].c_str(), is_selected)) {
					selectedWeaponRarity_ = n;
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

void EnemyManager::CreateJsonData(LWP::Utility::JsonIO& json, EnemyData& data, const std::string& name) {
	// ファイル名
	std::string fileName = name + ".json";
	json.Init(fileName)
		// ビヘイビアツリーのファイル名
		.AddValue<std::string>("BTFileName", &data.BTFileName)
		// 武器設定
		.BeginGroup("Weapon")
		// 手
		.BeginGroup("Hand")
		.BeginGroup("Left")
		.AddValue<int>("Type", &data.containWeaponTypes[(int)WeaponSide::kLeft])
		.EndGroup()
		.BeginGroup("Right")
		.AddValue<int>("Type", &data.containWeaponTypes[(int)WeaponSide::kRight])
		.EndGroup()
		.EndGroup()

		// 肩
		.BeginGroup("Shoulder")
		.BeginGroup("Left")
		.AddValue<int>("Type", &data.containWeaponTypes[(int)WeaponSide::kLeftShoulder])
		.EndGroup()
		.BeginGroup("Right")
		.AddValue<int>("Type", &data.containWeaponTypes[(int)WeaponSide::kRightShoulder])
		.EndGroup()
		.EndGroup()
		// 武器の最低保証
		.AddValue<int>("MinRarity", &data.minWeaponRarity)
		.EndGroup()
		// 攻撃パラメータ
		.BeginGroup("Attack")
		// 係数
		.AddValue<float>("Multiply", &data.attackMultiply)
		.EndGroup()
		// 速度倍率
		.AddValue<float>("SpeedMultiply", &data.speedMultiply)
		// 体力
		.AddValue<float>("HP", &data.hp)
		// レベル
		.AddValue<int>("Level", &data.level)

		.CheckJsonFile();
}

void EnemyManager::SelectEnemyDataGui(LWP::Utility::JsonIO& json, EnemyData& data) {
	// 調整項目
	if (ImGui::TreeNode("Json")) {
		if (ImGui::Button("Save")) {
			json.Save();
		}
		if (ImGui::Button("Load")) {
			json.Load();
		}

		// 調整する敵の種類
		data.type = selectCreateEnemyType_;

		// 使用するビヘイビアツリーを選択
		SelectJsonFile();
		data.BTFileName = enemyBTFileNamePreview_[selectBTFileName_];
		// 武器
		if (ImGui::TreeNode("Weapon")) {
			weaponTypePreview_ = WeaponManager::GetInstance()->GetWeaponTypePreview();
			weaponTypePreview_.push_back("None");
			weaponRarityPreview_ = WeaponManager::GetInstance()->GetWeaponRarityPreview();
			// 手武器を選択
			if (ImGui::TreeNode("Select hand weapon")) {
				SelectWeaponType(data.containWeaponTypes[(int)WeaponSide::kLeft], "Left");

				SelectWeaponType(data.containWeaponTypes[(int)WeaponSide::kRight], "Right");

				ImGui::TreePop();
			}
			// 肩武器を選択
			if (ImGui::TreeNode("Select shoulder weapon")) {
				SelectWeaponType(data.containWeaponTypes[(int)WeaponSide::kLeftShoulder], "Left");

				SelectWeaponType(data.containWeaponTypes[(int)WeaponSide::kRightShoulder], "Right");

				ImGui::TreePop();
			}
			// 最低保証のレアリティ
			ImGui::Text("Select weapon min rarity");
			SelectWeaponRarity();
			data.minWeaponRarity = selectedWeaponRarity_;

			ImGui::TreePop();
		}
		// 攻撃
		if (ImGui::TreeNode("Attack")) {
			// 倍率
			ImGui::DragFloat("Multiply", &data.attackMultiply, 0.1f, 0.0f);
			ImGui::TreePop();
		}
		// 速度倍率
		ImGui::DragFloat("SpeedMultiply", &data.speedMultiply, 0.01f, 0.0f);
		// HP
		ImGui::DragFloat("HP", &data.hp, 0.1f);
		// レアリティ
		ImGui::DragInt("Level", &data.level, 1, -1);

		ImGui::TreePop();
	}
}

void EnemyManager::SetWeaponPos(IWeapon* weapon, int weaponSide) {
	// 左手
	if ((int)WeaponSide::kLeft == weaponSide) {
		weapon->SetTranslation(Vector3{ -1.0f, -0.5f, 2.0f });
	}
	// 右手
	if ((int)WeaponSide::kRight == weaponSide) {
		weapon->SetTranslation(Vector3{ 1.0f, -0.5f, 2.0f });
	}
	// 左肩
	if ((int)WeaponSide::kLeftShoulder == weaponSide) {
		weapon->SetTranslation(Vector3{ -1.0f, 0.5f, 2.0f });
	}
	// 右肩
	if ((int)WeaponSide::kRightShoulder == weaponSide) {
		weapon->SetTranslation(Vector3{ 1.0f, 0.5f, 2.0f });
	}
	weapon->SetRotation(Quaternion{ 0,0,0,1 });
}