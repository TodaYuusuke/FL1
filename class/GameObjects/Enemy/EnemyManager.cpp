#include "EnemyManager.h"
#include "Melee/MeleeAttacker.h"
#include "Gunner/Gunner.h"
#include "Drone/Drone.h"
#include "Cargo/Cargo.h"
#include "Test/TestEnemy.h"
#include "../UI/ScoreUI/ScoreManager.h"
#include "../PenetrationResolver/PenetrationResolver.h"
#include "../World/IWorld.h"
#include "../Weapon/Gun/MachineGun/MachineGun.h"
#include "../Weapon/WeaponManager.h"
#include "../WaveManager.h"
#include "../../../DirectXGame/Externals/nlohmann/json.hpp"

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
	spawnType_ = EnemySpawnType::kNone;
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
	SpawnEnemy();

	// 更新
	for (Actor* actor : enemies_) {
		actor->Update();
		appendMiniMap_(actor->GetWorldTF()->GetWorldPosition());
	}
}

void EnemyManager::EndFrame() {
	// 削除
	enemies_.erase(
		std::remove_if(enemies_.begin(), enemies_.end(),
			[&](Actor* actor) {
				// エリア外にいったら削除
				if (actor->GetIsLimitMoveArea()) {
					// 武器を落とす
					for (int i = 0; i < actor->GetWeapon().size(); i++) {
						WeaponManager::GetInstance()->DeleteWeapon(actor->GetWeapon()[i]);
					}
					// 敵の解放
					delete actor;
					actor = nullptr;
					return true;
				}

				// 死んだら削除
				if (!actor->GetIsAlive()) {
					isTriggerDelete_ = true;
					killCount_++;

					// 武器を落とす
					for (int i = 0; i < actor->GetWeapon().size(); i++) {
						WeaponManager::GetInstance()->DropWeapon(actor->GetWeapon()[i]);
					}
					//スコアの増加
					ScoreCounter::GetInstance()->AddScore(actor->GetEnemyData().score);
					ScoreCounter::GetInstance()->AddKillCount();

					// 敵の解放
					delete actor;
					actor = nullptr;
					return true; // vectorから削除対象
				}
				return false;
			}),
		enemies_.end()
	);

	spawnEffects_.erase(
		std::remove_if(spawnEffects_.begin(), spawnEffects_.end(),
			[&](SpawnEffectData data) {
				if (data.currentFrame <= 0.0f) {
					return true; // vectorから削除対象
				}
				return false;
			}),
		spawnEffects_.end()
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

			ImGui::Text("Select level");
			static int level = 0;
			SelectType(levelPreview_, level, "Level##0");
			// 調整するレベル
			SelectLevelGui(levelJsonDatas_[level], sampleLevels_[level]);

			ImGui::Text("\nSelect enemy");
			static int enemyType = 0;
			SelectEnemyType(sampleEnemies_, enemyType, "EnemyType");
			// 選択した敵を調整
			SelectEnemyGui(jsonDatas_[enemyType], sampleEnemies_[enemyType]);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Spawn Edit")) {
			static int jsonType = 0;
			std::vector<std::string> spawnJsonName;
			// 読み込み
			spawnJsonName = GetFileNames((GetExeDir() / "resources/json/SpawnEnemy/").string());
			// 読み込むwaveのjsonファイル選択
			SelectType(spawnJsonName, jsonType, "SpawnJson");

			// ロードボタン
			if (ImGui::Button(("Json Load"))) {
				LoadSpawnJsonButton((GetExeDir() / "resources/json/SpawnEnemy/").string() + spawnJsonName[jsonType]);
				// モデル読み込み
				for (EnemySpawnData& data : spawnDatas_) {
					data.debugModel.LoadFullPath(EnemyConfig::ModelName::modelName[data.type]);
					data.debugModel.isActive = true;

					Vector3 pos = data.pos;
					// ドローンだけ高さ分引く
					if (data.type == (int)EnemyType::kDrone) {
						pos.y -= dronefloatHeight_;
					}
					data.debugModel.worldTF.translation = pos;
				}
			}
			ImGui::SameLine();
			// 保存ボタン
			if (ImGui::Button(("Json Export"))) {
				ExportSpawnJsonButton();
			}

			// 配置した敵の調整
			SpawnedEnemiesGui();
			// これから配置する敵の調整
			SpawnEnemyGui();

			// 編集モードボタン
			if (ImGui::Button("Start spawn edit")) {
				spawnType_ = EnemySpawnType::kNone;
				currentFrame_ = 0.0f;
				spawnInterval_ = 0.0f;
				spawnModel_.isActive = true;
			}

			ImGui::TreePop();
		}

		// 各敵のbehaviorTreeの調整
		if (ImGui::TreeNode("BT Edit")) {
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
		if (ImGui::Button("RandomSpawn")) {
			spawnType_ = EnemySpawnType::kRandom;
			currentFrame_ = 0.0f;
			spawnInterval_ = 0.0f;
			spawnDatas_.clear();
			spawnModel_.isActive = false;
		}
		if (ImGui::Button("WaveSpawn")) {
			spawnType_ = EnemySpawnType::kWave;
			currentFrame_ = 0.0f;
			spawnInterval_ = 0.0f;
			spawnDatas_.clear();
			spawnModel_.isActive = false;
		}

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

void EnemyManager::GiveWeapon(Actor* actor, const EnemyData& data) {
	for (int i = 0; i < data.containWeaponTypes.size(); i++) {
		if (data.containWeaponTypes[i] >= (int)WeaponType::kCount) { continue; }

		// 持たせる武器を作成
		std::vector<std::string> weaponNames = WeaponManager::GetInstance()->GetWeaponNamePreview(data.containWeaponTypes[i]);
		std::vector<int> weaponTypes;
		weaponTypes.push_back(data.containWeaponTypes[i]);
		std::vector<int> weaponRarities = {
			(int)RarityType::kCommon,
			(int)RarityType::kUnCommon,
			(int)RarityType::kRare,
			(int)RarityType::kSuperRare,
			(int)RarityType::kLegendary
		};
		IWeapon* weapon = WeaponManager::GetInstance()->CreateRandomWeapon(weaponTypes, weaponRarities, weaponRarityPercent_);
		// 所持者の攻撃倍率を武器に反映
		weapon->SetAttackMultiply(actor->GetEnemyData().attackMultiply);

		// 武器の付与
		WeaponManager::GetInstance()->PickUpWeapon(weapon, actor, i);

		// 武器の装着位置設定
		//SetWeaponPos(actor, weapon, i);
	}
}

void EnemyManager::SpawnEnemy() {
	// 敵が多すぎるなら出現させない
	if ((int)enemies_.size() >= limitSpawnNum_) { return; }

	// 次の敵を強制出現 or 敵がすべていない
	switch (spawnType_) {
	case EnemySpawnType::kRandom:
		currentFrame_ += HitStopController::GetInstance()->GetDeltaTime();
		spawnInterval_ -= HitStopController::GetInstance()->GetDeltaTime();

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
		break;
	case EnemySpawnType::kWave:
		currentFrame_ += HitStopController::GetInstance()->GetDeltaTime();
		spawnInterval_ -= HitStopController::GetInstance()->GetDeltaTime();

		// 生成したすべての敵を倒したら次のWaveに進む
		if (spawnInterval_ <= 0.0f && enemies_.empty()) {
			// 次のウェーブに移行
			WaveManager::GetInstance()->StartNextWave();
			weaponRarityPercent_ = WaveManager::GetInstance()->GetCurrentDropWeaponPercent();

			std::vector<std::string> spawnJsonName;
			spawnJsonName = GetFileNames((GetExeDir() / "resources/json/SpawnEnemy/").string());
			int index = LWP::Utility::Random::GenerateInt(0, (int)spawnJsonName.size() - 1);
			// jsonファイル読み込み
			LoadSpawnJsonButton((GetExeDir() / "resources/json/SpawnEnemy/").string() + spawnJsonName[index]);

			spawnInterval_ = spawnDatas_.back().spawnTime * 60.0f;
		}
		else {
			break;
		}

		// 生成開始
		for (EnemySpawnData& data : spawnDatas_) {
			if (data.isSpawn) { continue; }

			float lastSpawnTime = spawnDatas_.back().spawnTime;
			if (data.spawnTime <= (lastSpawnTime * 60.0f) - spawnInterval_) {
				data.isSpawn = true;
				selectCreateEnemyType_ = data.type;
				createPos_ = data.pos;
				enemies_.push_back(CreateEnemy());
			}
		}

		break;
	default:
		break;
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

void EnemyManager::CreateSpawnEffect(const LWP::Math::Vector3& pos) {
	SpawnEffectData data{};
	data.pos = pos;
	data.currentFrame = spawnEffectTime;
	//data.spawnEffectModels[0].LoadFullPath("resources/")

	spawnEffects_.push_back(data);
}

// ------------ デバッグ用関数↓------------ //

Actor* EnemyManager::CreateTestEnemy() {
	// テスト敵
	TestEnemy* actor = new TestEnemy();
	actor->SetTranslation(createPos_);
	actor->SetVelocity(createVel_);

	return actor;
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

void EnemyManager::SelectType(std::vector<std::string> list, int& selectedType, std::string label, bool& isClickCombo) {
	isClickCombo = false;
	if (!list.empty()) {
		const char* combo_preview_value = list[selectedType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < list.size(); n++) {
				const bool is_selected = ((int)selectedType == n);
				std::string selectableLabel = list[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedType = n;
					isClickCombo = true;
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

void EnemyManager::SelectEnemyType(std::map<int, EnemyData> data, int& selectType, const std::string& label) {
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
		.AddValue<int>("Name", &data.containWeaponNames[(int)WeaponSide::kLeft])
		.EndGroup()
		.BeginGroup("Right")
		.AddValue<int>("Type", &data.containWeaponTypes[(int)WeaponSide::kRight])
		.AddValue<int>("Name", &data.containWeaponNames[(int)WeaponSide::kRight])
		.EndGroup()
		.EndGroup()

		// 肩
		.BeginGroup("Shoulder")
		.BeginGroup("Left")
		.AddValue<int>("Type", &data.containWeaponTypes[(int)WeaponSide::kLeftShoulder])
		.AddValue<int>("Name", &data.containWeaponNames[(int)WeaponSide::kLeftShoulder])
		.EndGroup()
		.BeginGroup("Right")
		.AddValue<int>("Type", &data.containWeaponTypes[(int)WeaponSide::kRightShoulder])
		.AddValue<int>("Name", &data.containWeaponNames[(int)WeaponSide::kRightShoulder])
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

void EnemyManager::SelectEnemyGui(LWP::Utility::JsonIO& json, EnemyData& data) {
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
		SelectType(enemyBTFileNamePreview_, modifyBTFile_, "JsonFile##1");

		// 武器
		if (ImGui::TreeNode("Weapon")) {
			weaponTypePreview_ = WeaponManager::GetInstance()->GetWeaponTypePreview();
			weaponTypePreview_.push_back("None");
			//weaponRarityPreview_ = WeaponManager::GetInstance()->GetWeaponRarityPreview();
			// 手武器を選択
			if (ImGui::TreeNode("Select hand weapon")) {
				std::vector<std::string> lWeaponNamePreview;
				std::vector<std::string> rWeaponNamePreview;
				bool isClickCombo = false;
				int preType;

				preType = data.containWeaponTypes[(int)WeaponSide::kLeft];
				SelectType(weaponTypePreview_, data.containWeaponTypes[(int)WeaponSide::kLeft], "Left##0", isClickCombo);
				// タブクリック時の処理
				if (isClickCombo) {
					if (data.containWeaponTypes[(int)WeaponSide::kLeft] != preType) { data.containWeaponNames[(int)WeaponSide::kLeft] = 0; }
				}

				lWeaponNamePreview = WeaponManager::GetInstance()->GetWeaponNamePreview(data.containWeaponTypes[(int)WeaponSide::kLeft]);
				SelectType(lWeaponNamePreview, data.containWeaponNames[(int)WeaponSide::kLeft], "Left##1");


				preType = data.containWeaponTypes[(int)WeaponSide::kRight];
				SelectType(weaponTypePreview_, data.containWeaponTypes[(int)WeaponSide::kRight], "Right##0");
				// タブクリック時の処理
				if (isClickCombo) {
					if (data.containWeaponTypes[(int)WeaponSide::kRight] != preType) { data.containWeaponNames[(int)WeaponSide::kRight] = 0; }
				}

				rWeaponNamePreview = WeaponManager::GetInstance()->GetWeaponNamePreview(data.containWeaponTypes[(int)WeaponSide::kRight]);
				SelectType(rWeaponNamePreview, data.containWeaponNames[(int)WeaponSide::kRight], "Right##1");

				ImGui::TreePop();
			}
			// 肩武器を選択
			if (ImGui::TreeNode("Select shoulder weapon")) {
				std::vector<std::string> lWeaponNamePreview;
				std::vector<std::string> rWeaponNamePreview;
				bool isClickCombo = false;
				int preType;

				preType = data.containWeaponTypes[(int)WeaponSide::kLeftShoulder];
				SelectType(weaponTypePreview_, data.containWeaponTypes[(int)WeaponSide::kLeftShoulder], "Left##2");
				// タブクリック時の処理
				if (isClickCombo) {
					if (data.containWeaponTypes[(int)WeaponSide::kLeftShoulder] != preType) { data.containWeaponNames[(int)WeaponSide::kLeftShoulder] = 0; }
				}

				lWeaponNamePreview = WeaponManager::GetInstance()->GetWeaponNamePreview(data.containWeaponTypes[(int)WeaponSide::kLeftShoulder]);
				SelectType(lWeaponNamePreview, data.containWeaponNames[(int)WeaponSide::kLeftShoulder], "Left##1");

				preType = data.containWeaponTypes[(int)WeaponSide::kRightShoulder];
				SelectType(weaponTypePreview_, data.containWeaponTypes[(int)WeaponSide::kRightShoulder], "Right##2");
				// タブクリック時の処理
				if (isClickCombo) {
					if (data.containWeaponTypes[(int)WeaponSide::kRightShoulder] != preType) { data.containWeaponNames[(int)WeaponSide::kRightShoulder] = 0; }
				}

				rWeaponNamePreview = WeaponManager::GetInstance()->GetWeaponNamePreview(data.containWeaponTypes[(int)WeaponSide::kRightShoulder]);
				SelectType(rWeaponNamePreview, data.containWeaponNames[(int)WeaponSide::kRightShoulder], "Right##1");

				ImGui::TreePop();
			}

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

void EnemyManager::SpawnedEnemiesGui() {
	if (ImGui::TreeNode("SpawnedEnemies")) {
		// 編集モードでない
		if (spawnType_ != EnemySpawnType::kNone) {
			ImGui::Text("Not spawn edit mode. Click -Start spawn edit- button");
			ImGui::TreePop();
			return;
		}
		// 配置している敵がいない
		if (spawnDatas_.empty()) {
			ImGui::Text("Not spawn enemy");
			ImGui::TreePop();
			return;
		}

		// リストのソート
		if (ImGui::Button("Sort Spawn Time")) {
			SortSpawnTime();
		}

		static int type = 0;
		std::vector<std::string> preview;
		for (EnemySpawnData& data : spawnDatas_) {
			for (auto& key : data.debugModel.materials) {
				key.second.color.R = 255;
				key.second.color.G = 255;
				key.second.color.B = 255;
				key.second.color.A = 255;
			}

			preview.push_back(EnemyConfig::Name::name[data.type] + std::to_string(data.id));
		}

		// 調整する敵選択
		SelectType(preview, type, "SpawnEnemy");

		// 色を赤色にする
		for (auto& key : spawnDatas_[type].debugModel.materials) {
			key.second.color.R = 255;
			key.second.color.G = 0;
			key.second.color.B = 0;
		}

		std::string label = EnemyConfig::Name::name[spawnDatas_[type].type];
		if (ImGui::TreeNode(label.c_str())) {
			ImGui::DragFloat("SpawnTime", &spawnDatas_[type].spawnTime, 0.01f, 0.0f);
			ImGui::DragFloat3("Pos", &spawnDatas_[type].pos.x, 0.01f);
			spawnDatas_[type].debugModel.worldTF.translation = spawnDatas_[type].pos;
			ImGui::TreePop();
		}

		// 削除
		if (ImGui::Button("Delete")) {
			spawnDatas_.erase(spawnDatas_.begin() + type);
			type = 0;
		}

		ImGui::TreePop();
	}
}

void EnemyManager::SpawnEnemyGui() {
	if (ImGui::TreeNode("Spawn")) {
		// 編集モードでない
		if (spawnType_ != EnemySpawnType::kNone) {
			ImGui::Text("Not spawn edit mode. Click -Start spawn edit- button");
			ImGui::TreePop();
			return;
		}

		static int type = 0;
		static int preType = -1;
		static int placementID = 0;
		static Vector3 pos;
		static float spawnTime;
		SelectType(enemyTypePreview_, type, "EnemyType##1");
		ImGui::DragFloat("SpawnTime", &spawnTime, 0.01f, 0.0f);
		ImGui::DragFloat3("Pos", &pos.x, 0.01f);

		// 配置予定モデル表示
		spawnModel_.isActive = true;
		if (preType != type) {
			spawnModel_.LoadFullPath(EnemyConfig::ModelName::modelName[type]);
			for (auto& key : spawnModel_.materials) {
				key.second.color.R = 0;
				key.second.color.G = 255;
				key.second.color.B = 0;
				key.second.color.A = 100;
			}
		}
		spawnModel_.worldTF.translation = pos;
		if (type == (int)EnemyType::kDrone) {
			spawnModel_.worldTF.translation.y += dronefloatHeight_;
		}

		// 作成
		if (ImGui::Button("Done")) {
			spawnDatas_.emplace_back();
			spawnDatas_.back().isSpawn = false;
			spawnDatas_.back().type = type;
			spawnDatas_.back().id = placementID;
			spawnDatas_.back().spawnTime = spawnTime;
			spawnDatas_.back().pos = pos;
			if (type == (int)EnemyType::kDrone) {
				spawnDatas_.back().pos.y += dronefloatHeight_;
			}
			spawnDatas_.back().debugModel.LoadFullPath(EnemyConfig::ModelName::modelName[type]);
			spawnDatas_.back().debugModel.worldTF.translation = spawnDatas_.back().pos;

			placementID++;
		}

		preType = type;

		ImGui::TreePop();
	}
	else {
		spawnModel_.isActive = false;
	}
}

void EnemyManager::SortSpawnTime() {
	if (spawnDatas_.empty()) { return; }

	std::sort(spawnDatas_.begin(), spawnDatas_.end(),
		[](EnemySpawnData data, EnemySpawnData nextData) {
			return data.spawnTime < nextData.spawnTime;
		});
}

void EnemyManager::LoadSpawnJsonButton(const std::string& fileName) {
	OPENFILENAMEA ofn = { 0 };
	char szFile[MAX_PATH] = { 0 };	// ファイルパスのサイズはWindows既定のものに
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	std::string name = fileName;
	ofn.lpstrFilter = name.c_str();
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST;

	LoadSpawnJson(name);
	// 出現順にソート
	SortSpawnTime();
}

void EnemyManager::LoadSpawnJson(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) return;

	spawnDatas_.clear();
	int id = 0;
	nlohmann::json j;
	file >> j;
	for (const auto& node_json : j) {
		float spawnTime = node_json["spawnTime"];
		int type = node_json["type"];
		Vector3 createPos = {
			(float)node_json["createPos_x"],
			(float)node_json["createPos_y"],
			(float)node_json["createPos_z"]
		};
		EnemySpawnData data = {
			.isSpawn = false,
			.type = type,
			.id = id,
			.spawnTime = spawnTime,
			.pos = createPos
		};
		data.debugModel.LoadCube();
		data.debugModel.isActive = false;

		id++;

		spawnDatas_.push_back(data);
	}

	file.close();
}

void EnemyManager::ExportSpawnJsonButton() {
	// ファイル保存ダイアログを使って保存先とファイル名を指定
#if defined(_WIN32)
	char currentDir[MAX_PATH];
	_getcwd(currentDir, MAX_PATH);

	OPENFILENAMEA ofn = { 0 };
	char szFile[MAX_PATH] = { 0 };	// ファイルパスのサイズはWindows既定のものに
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrDefExt = "json";
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

	if (GetSaveFileNameA(&ofn)) {
		ExportSpawnJson(szFile);
	}

	_chdir(currentDir);
#endif
}

void EnemyManager::ExportSpawnJson(const std::string& fileName) {
	using std::swap;

	const int cJsonIndent = 4;
	nlohmann::json j;

	// 出現順にソート
	SortSpawnTime();
	for (EnemySpawnData& data : spawnDatas_) {
		nlohmann::json node_json;
		node_json["spawnTime"] = data.spawnTime;
		node_json["type"] = data.type;
		node_json["createPos_x"] = data.pos.x;
		node_json["createPos_y"] = data.pos.y;
		node_json["createPos_z"] = data.pos.z;

		j.push_back(node_json);
	}
	std::ofstream file(fileName);
	if (file.is_open()) {
		file << j.dump(cJsonIndent);
		file.close();
	}
}

std::filesystem::path EnemyManager::GetExeDir() {
	return std::filesystem::current_path();
}

std::vector<std::string> EnemyManager::GetFileNames(const std::string& folderPath) {
	std::vector<std::string> files;

	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.is_regular_file() &&
			entry.path().extension() == ".json") {
			files.push_back(entry.path().filename().string());
		}
	}

	return files;
}

void EnemyManager::TutorialSpawn() {
	// 調整情報
	EnemyData data = sampleEnemies_[(int)EnemyType::kMelee];
	data.BTFileName = "resources/json/BT/BT_Tutorial.json";
	data.attackMultiply = sampleLevels_[0].attackMultiply;
	data.speedMultiply = sampleLevels_[0].speedMultiply;
	data.level = sampleLevels_[0].value;
	data.type = (int)EnemyType::kMelee;

	// 近接敵
	MeleeAttacker* actor = new MeleeAttacker(pWorld_, createID_, data);
	// 武器を付与
	GiveWeapon(actor, sampleEnemies_[(int)EnemyType::kMelee]);

	// 武器の座標
	Vector3 enemyPos = pWorld_->FindActor("Player")->GetWorldTF()->GetWorldPosition();
	// 自機の向いている方向に敵を出す
	enemyPos += Vector3{ 0.0f,0.0f,1.0f } *Matrix4x4::CreateRotateXYZMatrix(pWorld_->FindActor("Player")->GetWorldTF()->rotation) * 40.0f;
	// 座標を指定
	actor->SetTranslation(enemyPos);
	enemies_.push_back(actor);

	createID_++;

	// 押し出し
	PenetrationResolver::GetInstance()->RegisterObject(actor);
}

void EnemyManager::SetWeaponPos(Actor* actor, IWeapon* weapon, int weaponSide) {
	weapon->SetRotation(Quaternion{ 0,0,0,1 });
	Vector3 weaponPos{};
	// 単一武器の場合
	if (actor->GetEnemyData().type == (int)EnemyType::kDrone) {
		weaponPos = actor->GetModel()->GetJointWorldPosition("WeaponAnchor");
		weapon->SetTranslation(weaponPos);
		return;
	}
	// 左手
	else if ((int)WeaponSide::kLeft == weaponSide) {
		weaponPos = actor->GetModel()->GetJointWorldPosition("WeaponAnchor.L");
		weapon->SetTranslation(weaponPos);
	}
	// 右手
	else if ((int)WeaponSide::kRight == weaponSide) {
		weaponPos = actor->GetModel()->GetJointWorldPosition("WeaponAnchor.R");
		weapon->SetTranslation(weaponPos);
	}
	// 左肩
	else if ((int)WeaponSide::kLeftShoulder == weaponSide) {
		weaponPos = actor->GetModel()->GetJointWorldPosition("ShoulderWeaponAnchor.L");
		weapon->SetTranslation(weaponPos);
	}
	// 右肩
	else if ((int)WeaponSide::kRightShoulder == weaponSide) {
		weaponPos = actor->GetModel()->GetJointWorldPosition("ShoulderWeaponAnchor.R");
		weapon->SetTranslation(weaponPos);
	}
}