#include "EnemyManager.h"
#include "Melee/MeleeAttacker.h"
#include "Gunner/Gunner.h"
#include "Drone/Drone.h"
#include "Cargo/Cargo.h"
#include "Test/TestEnemy.h"
#include "../Weapon/Gun/MachineGun/MachineGun.h"
#include "../Weapon/WeaponManager.h"
#include  "../World/IWorld.h"
#include "EnemyConfig.h"
#include "../UI/ScoreUI/ScoreManager.h"
#include "../PenetrationResolver/PenetrationResolver.h"

using namespace EnemyConfig;
using namespace LWP;
using namespace LWP::Math;

EnemyManager::EnemyManager(IWorld* world) {
	pWorld_ = world;

	// デバッグ用の情報を生成
	CreateDebugData();

#pragma region コピー元となる敵生成
	for (int i = 0; i < EnemyConfig::Name::name.size(); i++) {
		// モデル名を決める
		sampleEnemies_[i].modelName = EnemyConfig::ModelName::modelName[i];
		// 調整データ作成
		CreateJsonData(jsonDatas_[i], sampleEnemies_[i], EnemyConfig::Name::name[i]);
	}
#pragma endregion
	// 各敵のレベルの調整情報
	for (int i = 0; i < maxLevel_; i++) {
		CreateLevelJsonData(levelJsonDatas_[i], sampleLevels_[i], levelPreview_[i]);
	}

	// 管理クラスの調整項目
	json_.Init(kJsonDirectoryPath + "EnemyManager.json")
		.BeginGroup("SpawnData")
		.BeginGroup("Same")
		.AddValue<int>("Min", &minSameSpawnNum_)
		.AddValue<int>("Max", &maxSameSpawnNum_)
		.EndGroup()
		// 一度に存在できる数
		.AddValue<int>("LimitNum", &limitSpawnNum_)
		// 生成禁止範囲
		.AddValue<float>("InvalidRange", &invalidSpawnRange_)
		.EndGroup()
		// 各敵
		.BeginGroup("Enemy")
		.BeginGroup("Drone")
		.AddValue<float>("FloatHeight", &dronefloatHeight_)
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();

#ifdef _DEBUG
	isRandomSpawn_ = false;
#endif // DEBUG
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
	isTriggerDelete_ = false;

	// 敵を生成
	if (isRandomSpawn_) SpawnEnemy();

	// 更新
	for (Actor* actor : enemies_) {
		actor->Update();
	}
}

void EnemyManager::EndFrame() {
	// 削除
	enemies_.erase(
		std::remove_if(enemies_.begin(), enemies_.end(),
			[&](Actor* actor) {
				if (!actor->GetIsAlive()) {
					isTriggerDelete_ = true;
					killCount_++;

					// 武器を落とす
					for (int i = 0; i < actor->GetWeapon().size(); i++) {
						WeaponManager::GetInstance()->DropWeapon(actor->GetWeapon()[i]);
					}
					//スコアの増加
					ScoreCounter::GetInstance()->AddScore(actor->GetScore());

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
	if (ImGui::BeginTabItem("Enemies")) {
		// jsonによる調整データ
		if (ImGui::TreeNode("Json")) {
			// 管理クラス
			if (ImGui::TreeNode("RandomSpawnData")) {
				json_.DebugGUI();
				ImGui::TreePop();
			}

			static int level = 0;
			SelectType(levelPreview_, level, "Level##0");
			// 調整するレベル
			SelectLevelGui(levelJsonDatas_[level], sampleLevels_[level]);

			static int enemyType = 0;
			SelectCreateEnemy(sampleEnemies_, enemyType,"EnemyType");
			// 選択した敵を調整
			SelectEnemyDataGui(jsonDatas_[enemyType], sampleEnemies_[enemyType]);

			ImGui::TreePop();
		}

		// 各敵のbehaviorTreeの調整
		if (ImGui::TreeNode("BT edit")) {
			// 読み込むファイルを選択
			SwitchNodeEditorCanvas(btEditor_->GetEditorContext());
			SelectType(enemyBTFileNamePreview_, selectBTFileName_, "JsonFile##0");

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
			SelectType(enemyTypePreview_, selectCreateEnemyType_, "EnemyType##0");
			// 使用するレベルを選択
			SelectType(levelPreview_, selectLevel_, "Level##1");

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
				Actor* enemy = CreateEnemy();
				enemies_.push_back(enemy);
			}
			ImGui::TreePop();
		}

		// 経過時間
		ImGui::DragFloat("CurrentFrame", &currentFrame_);
		// 出現間隔
		ImGui::DragFloat("SpawnInterval", &spawnInterval_);
		// キル数
		ImGui::DragInt("KillCount", &killCount_);
		ImGui::DragInt("LimitSpawnNum", &limitSpawnNum_);

		// ランダム出現開始
		ImGui::Checkbox("GameStart!", &isRandomSpawn_);

		ImGui::EndTabItem();
	}
}

Actor* EnemyManager::CreateMeleeEnemy() {
	// 調整情報
	EnemyData data = sampleEnemies_[(int)EnemyType::kMelee];
	data.attackMultiply = sampleLevels_[selectLevel_].attackMultiply;
	data.speedMultiply = sampleLevels_[selectLevel_].speedMultiply;
	data.level = sampleLevels_[selectLevel_].value;
	data.type = (int)EnemyType::kMelee;

	// 近接敵
	MeleeAttacker* actor = new MeleeAttacker(pWorld_, createID_, data);

	// 武器を付与
	GiveWeapon(actor, sampleEnemies_[(int)EnemyType::kMelee]);

	// 座標を指定
	actor->SetTranslation(createPos_);

	createID_++;

	return actor;
}

Actor* EnemyManager::CreateGunnerEnemy() {
	// 調整情報
	EnemyData data = sampleEnemies_[(int)EnemyType::kGunner];
	data.attackMultiply = sampleLevels_[selectLevel_].attackMultiply;
	data.speedMultiply = sampleLevels_[selectLevel_].speedMultiply;
	data.level = sampleLevels_[selectLevel_].value;
	data.type = (int)EnemyType::kGunner;

	// 遠距離敵
	Gunner* actor = new Gunner(pWorld_, createID_, data);

	// 武器を付与
	GiveWeapon(actor, sampleEnemies_[(int)EnemyType::kGunner]);

	// 座標を指定
	actor->SetTranslation(createPos_);

	createID_++;

	return actor;
}

Actor* EnemyManager::CreateDroneEnemy() {
	// 調整情報
	EnemyData data = sampleEnemies_[(int)EnemyType::kDrone];
	data.attackMultiply = sampleLevels_[selectLevel_].attackMultiply;
	data.speedMultiply = sampleLevels_[selectLevel_].speedMultiply;
	data.level = sampleLevels_[selectLevel_].value;
	data.type = (int)EnemyType::kDrone;

	// ドローン敵
	Drone* actor = new Drone(pWorld_, createID_, data);

	// 武器を付与
	GiveWeapon(actor, sampleEnemies_[(int)EnemyType::kDrone]);

	// 座標を指定
	actor->SetTranslation(createPos_);
	actor->SetFloatHeight(dronefloatHeight_);

	createID_++;

	return actor;
}

Actor* EnemyManager::CreateCargoEnemy() {
	// 調整情報
	EnemyData data = sampleEnemies_[(int)EnemyType::kCargo];
	data.attackMultiply = sampleLevels_[selectLevel_].attackMultiply;
	data.speedMultiply = sampleLevels_[selectLevel_].speedMultiply;
	data.level = sampleLevels_[selectLevel_].value;
	data.type = (int)EnemyType::kCargo;

	// 遠距離敵
	Cargo* actor = new Cargo(pWorld_, createID_, data);

	// 武器を付与
	GiveWeapon(actor, sampleEnemies_[(int)EnemyType::kCargo]);

	// 座標を指定
	actor->SetTranslation(createPos_);

	createID_++;

	return actor;
}

void EnemyManager::GiveWeapon(Actor* actor, const EnemyData& data) {
	for (int i = 0; i < data.containWeaponTypes.size(); i++) {
		if (data.containWeaponTypes[i] >= (int)WeaponType::kCount) { continue; }

		// 持たせる武器を作成
		IWeapon* weapon = WeaponManager::GetInstance()->CreateWeapon(data.containWeaponTypes[i], 0);
		// 所持者の攻撃倍率を武器に反映
		weapon->SetAttackMultiply(actor->GetEnemyData().attackMultiply);

		// 武器の付与
		WeaponManager::GetInstance()->PickUpWeapon(weapon, actor, i);

		// 武器の装着位置設定
		SetWeaponPos(actor, weapon, i);
	}
}

void EnemyManager::SpawnEnemy() {
	// 敵が多すぎるなら出現させない
	if ((int)enemies_.size() >= limitSpawnNum_) { return; }

	currentFrame_ += HitStopController::GetInstance()->GetDeltaTime();
	spawnInterval_ -= HitStopController::GetInstance()->GetDeltaTime();

	// 次の敵を強制出現 or 敵がすべていない
	if (spawnInterval_ <= 0.0f || enemies_.empty()) {
		spawnInterval_ = spawnData_.nextSpawnTime * 60.0f;
		spawnData_.spawnNum = LWP::Utility::Random::GenerateInt(minSameSpawnNum_, maxSameSpawnNum_);
		// 生成開始
		for (int i = 0; i < spawnData_.spawnNum; i++) {
			selectCreateEnemyType_ = LWP::Utility::Random::GenerateInt(0, (int)EnemyType::kCount - 2);
			CreateRandomSpawnPos();
			enemies_.push_back(CreateEnemy());
		}
	}
}

void EnemyManager::CreateRandomSpawnPos() {
	// 自機の座標を確認
	Actor* actor = pWorld_->FindActor("Player");

	// 自機座標の近くには生成しない
	float max = 1.0f;
	Vector3 randomSpawnDir = LWP::Utility::Random::GenerateVector3(Vector3{ -max,-max,-max }, Vector3{ max,max,max }).Normalize();
	randomSpawnDir.y = 0.0f;
	randomSpawnDir *= invalidSpawnRange_;
	createPos_ = actor->GetWorldTF()->GetWorldPosition() + randomSpawnDir;
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
	Actor* enemy = nullptr;
	switch (selectCreateEnemyType_) {
		// 近接
	case (int)EnemyType::kMelee:
		enemy = CreateMeleeEnemy();
		PenetrationResolver::GetInstance()->RegisterObject(enemy);
		return enemy;
		break;
		// 遠距離
	case (int)EnemyType::kGunner:
		enemy = CreateGunnerEnemy();
		PenetrationResolver::GetInstance()->RegisterObject(enemy);
		return enemy;
		break;
	case (int)EnemyType::kDrone:
		enemy = CreateDroneEnemy();
		PenetrationResolver::GetInstance()->RegisterObject(enemy);
		return enemy;
		break;
	case (int)EnemyType::kCargo:
		enemy = CreateCargoEnemy();
		PenetrationResolver::GetInstance()->RegisterObject(enemy);
		return enemy;
		break;
		// テスト
	case (int)EnemyType::kTest:
		enemy = CreateTestEnemy();
		PenetrationResolver::GetInstance()->RegisterObject(enemy);
		return enemy;
		break;
	default:
		return nullptr;
		break;
	}
}

void EnemyManager::SelectType(std::vector<std::string> list, int& selectedType, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!list.empty()) {
		const char* combo_preview_value = list[selectedType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < list.size(); n++) {
				const bool is_selected = ((int)selectedType == n);
				std::string selectableLabel = list[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedType = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found element"));
	}
}

void EnemyManager::SelectCreateEnemy(std::map<int, EnemyData> data, int& selectType, const std::string& label) {
	ImGui::Text("Select create enemy");
	// 追加する敵のプレビュー作成
	if (!enemyTypePreview_.empty()) {
		const char* combo_preview_value = enemyTypePreview_[selectType].c_str();
		if (ImGui::BeginCombo((label.c_str()), combo_preview_value)) {
			for (int n = 0; n < enemyTypePreview_.size(); n++) {
				const bool is_selected = ((int)selectType == n);
				if (ImGui::Selectable(enemyTypePreview_[n].c_str(), is_selected)) {
					selectType = n;
					modifyBTFile_ = EnemyConfig::BTFileName::GetBTNameIndex(data[n].BTFileName);
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

void EnemyManager::SelectJsonFile(EnemyData& data, int& selectedType, const std::string& label) {
	ImGui::Text("Select Behavior-Tree file");
	// 読み込むbehaviorTreeのプレビュー作成
	if (!enemyBTFileNamePreview_.empty()) {
		const char* combo_preview_value = enemyBTFileNamePreview_[selectedType].c_str();
		if (ImGui::BeginCombo((label.c_str()), combo_preview_value)) {
			for (int n = 0; n < enemyBTFileNamePreview_.size(); n++) {
				const bool is_selected = ((int)selectedType == n);
				if (ImGui::Selectable(enemyBTFileNamePreview_[n].c_str(), is_selected)) {
					selectedType = n;
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

void EnemyManager::SelectLevel(const std::string& label) {
	ImGui::Text("Select level");
	// 読み込むbehaviorTreeのプレビュー作成
	if (!levelPreview_.empty()) {
		const char* combo_preview_value = levelPreview_[selectLevel_].c_str();
		if (ImGui::BeginCombo((label.c_str()), combo_preview_value)) {
			for (int n = 0; n < levelPreview_.size(); n++) {
				const bool is_selected = ((int)selectLevel_ == n);
				if (ImGui::Selectable(levelPreview_[n].c_str(), is_selected)) {
					selectLevel_ = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found level"));
	}
}

void EnemyManager::SelectWeaponType(int& selectedWeaponType, std::string label) {
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

void EnemyManager::SelectWeaponRarity() {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponRarityPreview_.empty()) {
		const char* combo_preview_value = weaponRarityPreview_[selectedWeaponRarity_].c_str();
		if (ImGui::BeginCombo(("Weapon rarity"), combo_preview_value)) {
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
	// レベルプレビュー
	for (int i = 1; i <= maxLevel_; i++) {
		levelPreview_.push_back("Level" + std::to_string(i));
	}
#pragma endregion
}

void EnemyManager::CreateJsonData(LWP::Utility::JsonIO& json, EnemyData& data, const std::string& jsonName) {
	// ファイル名
	std::string fileName = kJsonDirectoryPath + jsonName + ".json";
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
		// 当たり判定
		.BeginGroup("Collider")
		.AddValue<Vector3>("Min", &data.colliderMin)
		.AddValue<Vector3>("Max", &data.colliderMax)
		.EndGroup()
		// 体力
		.AddValue<float>("HP", &data.hp)
		// 得点
		.AddValue<float>("Score", &data.score)

		.CheckJsonFile();
}

void EnemyManager::CreateLevelJsonData(LWP::Utility::JsonIO& json, LevelParameter& data, const std::string& jsonName) {
	// ファイル名
	std::string fileName = kLevelJsonDirectoryPath + jsonName + ".json";
	json.Init(fileName)
		// 係数
		.AddValue<float>("AttackMultiply", &data.attackMultiply)
		// 速度倍率
		.AddValue<float>("SpeedMultiply", &data.speedMultiply)
		// レベル
		.AddValue<int>("Level", &data.value)

		.CheckJsonFile();
}

void EnemyManager::SelectEnemyDataGui(LWP::Utility::JsonIO& json, EnemyData& data) {
	// 調整項目
	if (ImGui::TreeNode("Enemy")) {
		if (ImGui::Button("Save")) {
			data.BTFileName = enemyBTFileNamePreview_[modifyBTFile_];
			json.Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			json.Load();
		}

		// 使用するビヘイビアツリーを選択
		SelectJsonFile(data, modifyBTFile_, "JsonFile##1");

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

		// 当たり判定
		if (ImGui::TreeNode("Collider")) {
			ImGui::DragFloat3("Min", &data.colliderMin.x, 0.01f);
			ImGui::DragFloat3("Max", &data.colliderMax.x, 0.01f);
			ImGui::TreePop();
		}

		// HP
		ImGui::DragFloat("HP", &data.hp, 0.1f);

		// 得点
		ImGui::DragFloat("Score", &data.score, 0.1f);

		ImGui::TreePop();
	}
}

void EnemyManager::SelectLevelGui(LWP::Utility::JsonIO& json, LevelParameter& data) {
	// 調整項目
	if (ImGui::TreeNode("Level")) {
		if (ImGui::Button("Save")) {
			data.value = selectLevel_;
			json.Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			json.Load();
		}

		// 攻撃倍率
		ImGui::DragFloat("AttackMultiply", &data.attackMultiply, 0.1f, 0.0f);
		// 速度倍率
		ImGui::DragFloat("SpeedMultiply", &data.speedMultiply, 0.01f, 0.0f);

		ImGui::TreePop();
	}
}

void EnemyManager::SetWeaponPos(Actor* actor, IWeapon* weapon, int weaponSide) {
	weapon->SetRotation(Quaternion{ 0,0,0,1 });
	Vector3 weaponPos{};
	// 単一武器の場合
	if (actor->GetEnemyData().type == (int)EnemyType::kDrone) {
		weaponPos = actor->GetModel().GetJointWorldPosition("WeaponAnchor");
		weapon->SetTranslation(weaponPos);
		return;
	}
	// 左手
	else if ((int)WeaponSide::kLeft == weaponSide) {
		weaponPos = actor->GetModel().GetJointWorldPosition("WeaponAnchor.L");
		weapon->SetTranslation(weaponPos);
	}
	// 右手
	else if ((int)WeaponSide::kRight == weaponSide) {
		weaponPos = actor->GetModel().GetJointWorldPosition("WeaponAnchor.R");
		weapon->SetTranslation(weaponPos);
	}
	// 左肩
	else if ((int)WeaponSide::kLeftShoulder == weaponSide) {
		weaponPos = actor->GetModel().GetJointWorldPosition("ShoulderWeaponAnchor.L");
		weapon->SetTranslation(weaponPos);
	}
	// 右肩
	else if ((int)WeaponSide::kRightShoulder == weaponSide) {
		weaponPos = actor->GetModel().GetJointWorldPosition("ShoulderWeaponAnchor.R");
		weapon->SetTranslation(weaponPos);
	}
}