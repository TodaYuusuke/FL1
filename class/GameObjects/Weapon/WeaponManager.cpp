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
	CreateOrizinWeapon();
}

void WeaponManager::Init() {
	for (IWeapon* weapon : weapons_) {
		weapon->Init();
	}
}

void WeaponManager::Update() {
	// 自機が近いなら武器を渡す
	CheckPlayerToWeaponDistance();

	for (IWeapon* weapon : weapons_) {
		weapon->Update();
	}
}

void WeaponManager::DebugGui() {
	static WeaponData data{};

	if (ImGui::BeginTabItem("WeaponManager")) {
		// 調整項目
		if (ImGui::TreeNode("Json")) {
			// 武器種
			if (ImGui::TreeNode("Type")) {
				for (int i = 0; i < weaponTypePreview_.size(); i++) {
					ImGui::RadioButton(weaponTypePreview_[i].c_str(), &selectedOrizinWeaponType_, i);
				}
				ImGui::TreePop();
			}
			// レアリティ
			if (ImGui::TreeNode("Rarity")) {
				for (int i = 0; i < weaponRarityPreview_.size(); i++) {
					ImGui::RadioButton(weaponRarityPreview_[i].c_str(), &selectedOrizinRarityType_, i);
				}
				ImGui::TreePop();
			}

			// 選択した武器を調整
			orizinWeaponData_[(WeaponType)selectedOrizinWeaponType_][(RarityType)selectedOrizinRarityType_]->DebugGui();

			ImGui::TreePop();
		}

		// 武器作成
		if (ImGui::TreeNode("CreateWeapon")) {
			ImGui::Text("Select create weapon");
			// 武器種
			if (ImGui::TreeNode("Type")) {
				for (int i = 0; i < weaponTypePreview_.size(); i++) {
					ImGui::RadioButton(weaponTypePreview_[i].c_str(), &selectWeapon_, i);
				}
				ImGui::TreePop();
			}
			int selectRarity = 0;
			// レアリティ
			if (ImGui::TreeNode("Rarity")) {
				for (int i = 0; i < weaponRarityPreview_.size(); i++) {
					ImGui::RadioButton(weaponRarityPreview_[i].c_str(), &selectRarity, i);
				}
				ImGui::TreePop();
			}
			// 生成座標
			ImGui::DragFloat3("CreatePos", &createPos_.x, 0.01f);

			// 武器生成
			if (ImGui::Button("Create")) {
				// コピー元の武器情報
				createWeaponData_ = orizinWeaponData_[(WeaponType)selectWeapon_][(RarityType)selectRarity]->GetWeaponData();
				IWeapon* weapon = CreateSelectedWeapon(selectWeapon_);
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
		GiveWeapon(weapon, pWorld_->FindActor("Player"));
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

std::string WeaponManager::SelectWeaponModelName(int weaponType) {
	return WeaponConfig::ModelName::modelName[weaponType];
}

std::string WeaponManager::ConvertWeaponTypeName(int type) {
	return WeaponConfig::Name::name[type];
}

std::string WeaponManager::ConvertWeaponRarityName(int rarity) {
	return WeaponConfig::Rarity::rarity[rarity];
}

void WeaponManager::CreateOrizinWeapon() {
	for (int i = 0; i < weaponTypePreview_.size(); i++) {
		for (int j = 0; j < weaponRarityPreview_.size(); j++) {
			createWeaponData_.name = ConvertWeaponTypeName(i);
			createWeaponData_.modelName = SelectWeaponModelName(i);
			createWeaponData_.rarity = j;
			orizinWeaponData_[(WeaponType)i][(RarityType)j] = CreateSelectedWeapon(i);
		}
	}
	// 初期化(デバッグ時に使用するから)
	createWeaponData_ = {};

	// 武器種
	for (auto& type : orizinWeaponData_) {
		// 名前
		std::string weaponType = ConvertWeaponTypeName((int)type.first);
		// レアリティ
		for (auto& rarity : type.second) {
			// 名前
			std::string rarityType = ConvertWeaponRarityName((int)rarity.first);
			// コピー元になる武器作成
			rarity.second->CreateJsonData(weaponType + "_" + rarityType);
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

void WeaponManager::DropWeapon(IWeapon* weapon) {
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

void WeaponManager::GiveWeapon(IWeapon* weapon, Actor* target) {
	weapon->SetTranslation(Vector3{ 0,0,0 });
	weapon->SetRotation(Quaternion{ 0,0,0,1 });
	target->SetWeapon(weapon);
}

IWeapon* WeaponManager::CreateWeapon(int weaponType, int weaponRarity) {
	// コピー元の武器情報を設定
	createWeaponData_ = orizinWeaponData_[(WeaponType)weaponType][(RarityType)weaponRarity]->GetWeaponData();

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
	createWeaponData_ = orizinWeaponData_[(WeaponType)type][(RarityType)rarity]->GetWeaponData();

	// 選ばれた武器種作成
	IWeapon* weapon = CreateSelectedWeapon(type);
	// 武器リストに追加
	weapons_.push_back(weapon);

	return weapon;
}
