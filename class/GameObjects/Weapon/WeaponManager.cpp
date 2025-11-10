#include "WeaponManager.h"
#include "Gun/MachineGun/MachineGun.h"
#include "Gun/ShotGun/ShotGun.h"
#include "Gun/Rifle/Rifle.h"
#include "Gun/Launcher/Launcher.h"
#include "Gun/Missile/Missile.h"
#include "Melee/Melee.h"
#include "../World/World.h"
#include "../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;

WeaponManager::WeaponManager() {
	// 武器種生成プレビュー
	for (int i = 0; i < WeaponConfig::Rarity::rarity.size(); i++) {
		weaponTypePreview_.push_back(WeaponConfig::Name::name[i]);
	}
	// 武器レアリティ生成プレビュー
	for (int i = 0; i < WeaponConfig::Rarity::rarity.size(); i++) {
		weaponRarityPreview_.push_back(WeaponConfig::Rarity::rarity[i]);
	}

	// コピー元の武器作成
	CreateOriginWeapon();
}

WeaponManager::~WeaponManager() {
	// 現在の武器
	for (IWeapon* weapon : weapons_) {
		delete weapon;
	}
}

void WeaponManager::Init() {
	for (IWeapon* weapon : weapons_) {
		weapon->Init();
	}
}

void WeaponManager::Update() {
	for (IWeapon* weapon : weapons_) {
		weapon->Update();
	}

	// 自機が近いなら武器を渡す
	CheckPlayerToWeaponDistance();
}

void WeaponManager::DebugGui() {
	if (ImGui::BeginTabItem("WeaponManager")) {
		// 調整項目
		if (ImGui::TreeNode("Json")) {
			ImGui::Text("Select modify weapon");
			static int selectWeapon = 0;
			static int selectRarity = 0;
			// 武器種
			SelectWeaponType(selectWeapon, "Type");
			// レアリティ
			SelectWeaponRarity(selectRarity, "Rarity");

			// 選択した武器を調整
			SelectWeaponDataGui(jsonDatas_[(WeaponType)selectWeapon][(RarityType)selectRarity], orizinWeaponData_[(WeaponType)selectWeapon][(RarityType)selectRarity]);

			ImGui::TreePop();
		}

		// 武器作成
		if (ImGui::TreeNode("Create Weapon")) {
			ImGui::Text("Select create weapon");
			static int selectWeapon = 0;
			static int selectRarity = 0;
			// 武器種
			SelectWeaponType(selectWeapon, "Type");
			// レアリティ
			SelectWeaponRarity(selectRarity, "Rarity");

			// 生成座標
			ImGui::DragFloat3("CreatePos", &createPos_.x, 0.01f);

			// 武器生成
			if (ImGui::Button("Create")) {
				// コピー元の武器情報
				createWeaponData_ = orizinWeaponData_[(WeaponType)selectWeapon][(RarityType)selectRarity];
				IWeapon* weapon = CreateSelectedWeapon(selectWeapon);
				weapons_.push_back(weapon);
				// 武器を地面に出す
				DropWeapon(weapon);
			}

			ImGui::TreePop();
		}

		ImGui::EndTabItem();
	}
}

void WeaponManager::CheckPlayerToWeaponDistance() {
	for (IWeapon* weapon : weapons_) {
		// 所有者がいるなら終了
		if (weapon->GetActor()) { continue; }
		// 武器が可視化されていないなら終了
		if (!weapon->GetModel().isActive) { continue; }
		// 一定距離内にいるなら回収
		if (Vector3::Distance(player_->GetWorldTF()->GetWorldPosition(), weapon->GetWorldTF()->GetWorldPosition()) > 5.0f) { continue; }

		// 自機に武器を付与
		PickUpWeapon(weapon, pWorld_->FindActor("Player"));
	}
}

IWeapon* WeaponManager::CreateSelectedWeapon(int weaponType) {
	switch (weaponType) {
	case (int)WeaponType::kMachineGun:
		return CreateMachineGun();
		break;
	case (int)WeaponType::kShotGun:
		return CreateShotGun();
		break;
	case (int)WeaponType::kRifle:
		return CreateRifle();
		break;
	case (int)WeaponType::kLauncher:
		return CreateLauncher();
		break;
	case (int)WeaponType::kMissile:
		return CreateMissile();
		break;
	case (int)WeaponType::kMelee:
		return CreateMelee();
		break;
	default:
		return nullptr;
		break;
	}
}

std::string WeaponManager::ConvertWeaponModelName(int weaponType, int weaponRarity) {
	return WeaponConfig::ModelName::modelName[weaponType][weaponRarity];
}

std::string WeaponManager::ConvertWeaponTypeName(int type) {
	return WeaponConfig::Name::name[type];
}

std::string WeaponManager::ConvertWeaponRarityName(int rarity) {
	return WeaponConfig::Rarity::rarity[rarity];
}

void WeaponManager::CreateOriginWeapon() {
	for (int i = 0; i < weaponTypePreview_.size(); i++) {
		// 武器種名
		std::string weaponType = ConvertWeaponTypeName(i);
		for (int j = 0; j < weaponRarityPreview_.size(); j++) {
			// レアリティ名
			std::string rarityType = ConvertWeaponRarityName(j);
			orizinWeaponData_[(WeaponType)i][(RarityType)j].name = weaponType;
			orizinWeaponData_[(WeaponType)i][(RarityType)j].modelName = ConvertWeaponModelName(i, j);
			orizinWeaponData_[(WeaponType)i][(RarityType)j].rarity = j;

			// 調整データ作成
			CreateJsonData(jsonDatas_[(WeaponType)i][(RarityType)j], orizinWeaponData_[(WeaponType)i][(RarityType)j], weaponType + "_" + rarityType);
		}
	}
}

MachineGun* WeaponManager::CreateMachineGun() {
	MachineGun* gun = new MachineGun(createWeaponData_);
	return gun;
}

ShotGun* WeaponManager::CreateShotGun() {
	ShotGun* gun = new ShotGun(createWeaponData_);

	return gun;
}

Rifle* WeaponManager::CreateRifle() {
	Rifle* gun = new Rifle(createWeaponData_);

	return gun;
}

Launcher* WeaponManager::CreateLauncher() {
	Launcher* gun = new Launcher(createWeaponData_);

	return gun;
}

Missile* WeaponManager::CreateMissile() {
	Missile* gun = new Missile(createWeaponData_);

	return gun;
}

Melee* WeaponManager::CreateMelee() {
	Melee* gun = new Melee(createWeaponData_);

	return gun;
}

void WeaponManager::CreateJsonData(LWP::Utility::JsonIO& json, WeaponData& data, const std::string& name) {
	// ファイル名
	std::string fileName = name + ".json";
	json.Init(fileName)
		// 発射間隔
		.BeginGroup("Interval")
		.AddValue<float>("Normal", &data.shotIntervalTime)
		.AddValue<float>("Burst", &data.burstIntervalTime)
		.EndGroup()

		// 弾
		.BeginGroup("Bullet")
		// 弾数
		.AddValue<int>("Num", &data.bulletNum)
		// 同時に出る弾数
		.AddValue<int>("SameBulletNum", &data.sameBulletNum)
		// 拡散範囲
		.AddValue<Vector3>("DiffusingRange", &data.diffusingBulletRange)
		// 弾速
		.AddValue<float>("Speed", &data.bulletSpeed)
		.EndGroup()

		// バースト数
		.AddValue<int>("BurstNum", &data.burstNum)
		// 攻撃力
		.AddValue<float>("AttackPower", &data.attackValue)

		// 溜め時間
		.AddValue<float>("StoreTime", &data.storeTime)
		// 撃てない時間
		.AddValue<float>("CoolTime", &data.coolTime)
		// リロード時間
		.AddValue<float>("ReloadTime", &data.reloadTime)

		// レアリティ
		.AddValue<int>("Rarity", &data.rarity)
		.CheckJsonFile();
}

void WeaponManager::SelectWeaponDataGui(LWP::Utility::JsonIO& json, WeaponData& data) {
	// 調整項目
	if (ImGui::TreeNode("Json")) {
		if (ImGui::Button("Save")) {
			json.Save();
		}
		if (ImGui::Button("Load")) {
			json.Load();
		}

		// 発射間隔
		if (ImGui::TreeNode("Interval")) {
			ImGui::DragFloat("Normal", &data.shotIntervalTime, 0.01f, 0.0f);
			ImGui::DragFloat("Burst", &data.burstIntervalTime, 0.01f, 0.0f);
			ImGui::TreePop();
		}
		// 弾
		if (ImGui::TreeNode("Bullet")) {
			// 弾数
			ImGui::DragInt("Num", &data.bulletNum, 1, 0);
			// 同時に出る弾数
			ImGui::DragInt("SameNum", &data.sameBulletNum, 1, 0);
			// 弾の拡散範囲
			ImGui::DragFloat3("DiffusingRange", &data.diffusingBulletRange.x, 0.01f, 0.0001f, 1.0f);
			// 弾速
			ImGui::DragFloat("Speed", &data.bulletSpeed);
			ImGui::TreePop();
		}
		// バースト数
		ImGui::DragInt("BurstNum", &data.burstNum, 1, 0);
		// 攻撃力
		ImGui::DragFloat("AttackPower", &data.attackValue);
		// 溜め時間
		ImGui::DragFloat("StoreTime", &data.storeTime);
		// 撃てない時間
		ImGui::DragFloat("CoolTime", &data.coolTime);
		// リロード時間
		ImGui::DragFloat("ReloadTime", &data.reloadTime);

		ImGui::TreePop();
	}
}

void WeaponManager::SelectWeaponType(int& selectedWeaponType, std::string label) {
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

void WeaponManager::SelectWeaponRarity(int& selectedWeaponRarity, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponRarityPreview_.empty()) {
		const char* combo_preview_value = weaponRarityPreview_[selectedWeaponRarity].c_str();
		if (ImGui::BeginCombo((label.c_str()), combo_preview_value)) {
			for (int n = 0; n < weaponRarityPreview_.size(); n++) {
				const bool is_selected = ((int)selectedWeaponRarity == n);
				if (ImGui::Selectable(weaponRarityPreview_[n].c_str(), is_selected)) {
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
		ImGui::TextDisabled(("Not found behavior-tree file"));
	}
}

void WeaponManager::DropWeapon(IWeapon* weapon) {
	if (!weapon) return;

	// 武器の所持者がいる場合
	if (weapon->GetActor()) {
		Vector3 pos = weapon->GetActor()->GetWorldTF()->GetWorldPosition() + weapon->GetWorldTF()->translation;
		// 親子付け解除
		weapon->SetParent(nullptr);

		// 座標指定
		weapon->SetTranslation(pos);
	}
	// 所持者なし
	else {
		// 座標指定
		weapon->SetTranslation(createPos_);
	}
}

void WeaponManager::PickUpWeapon(IWeapon* weapon, Actor* target, int weaponSide) {
	weapon->Init();
	target->SetWeapon(weapon, weaponSide);
}

IWeapon* WeaponManager::CreateWeapon(int weaponType, int weaponRarity) {
	// コピー元の武器情報を設定
	createWeaponData_ = orizinWeaponData_[(WeaponType)weaponType][(RarityType)weaponRarity];

	// 選ばれた武器種作成
	IWeapon* weapon = CreateSelectedWeapon(weaponType);
	// 武器リストに追加
	weapons_.push_back(weapon);

	return weapon;
}

IWeapon* WeaponManager::CreateRandomWeapon(const std::vector<int>& weaponTypes, const std::vector<int>& weaponRarity) {
#pragma region 武器種
	int typeSize = (int)weaponTypes.size();
	int randomType = 0;
	int type = 0;
	// 1種類の場合
	if (typeSize == 1) {
		type = weaponTypes[0];
	}
	// ランダムで武器種を決める
	else if (typeSize >= 2) {
		randomType = LWP::Utility::Random::GenerateInt(0, typeSize - 1);
		type = weaponTypes[randomType];
	}
#pragma endregion

#pragma region レアリティ
	int raritySize = (int)weaponRarity.size();
	int randomRarity = 0;
	int rarity = 0;
	// 1種類の場合
	if (raritySize == 1) {
		rarity = weaponRarity[0];
	}
	// ランダムでレアリティを決める
	else if (raritySize >= 2) {
		randomRarity = LWP::Utility::Random::GenerateInt(0, raritySize - 1);
		rarity = weaponRarity[randomRarity];
	}
#pragma endregion

	// コピー元の武器情報を決定
	createWeaponData_ = orizinWeaponData_[(WeaponType)type][(RarityType)rarity];

	// 選ばれた武器種作成
	IWeapon* weapon = CreateSelectedWeapon(type);
	// 武器リストに追加
	weapons_.push_back(weapon);

	return weapon;
}

void WeaponManager::DeleteWeapon(IWeapon* weapon) {
	auto result = std::find(weapons_.begin(), weapons_.end(), weapon);
	// 存在しているなら削除
	if (result != weapons_.end()) {
		delete weapon;
		weapon = nullptr;
		weapons_.erase(result);
	}
}
