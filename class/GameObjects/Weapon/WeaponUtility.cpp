#include "WeaponUtility.h"
#include "Gun/MachineGun/MachineGun.h"
#include "Gun/ShotGun/ShotGun.h"
#include "Gun/Rifle/Rifle.h"
#include "Gun/Launcher/Launcher.h"
#include "Gun/Missile/Missile.h"
#include "Melee/Melee.h"

WeaponUtility::WeaponUtility() {
	// 武器生成プレビュー
	weaponPreview_.push_back("MachineGun");
	weaponPreview_.push_back("ShotGun");
	weaponPreview_.push_back("Rifle");
	weaponPreview_.push_back("Launcher");
	weaponPreview_.push_back("Missile");
	weaponPreview_.push_back("Melee");
}

void WeaponUtility::DrawGui() {
	ImGui::Text("Select create weapon");

	for (int i = 0; i < weaponPreview_.size(); i++) {
		ImGui::RadioButton(weaponPreview_[i].c_str(), &selectWeapon_, i);
		i++;
	}

	// 武器生成
	if (ImGui::Button("Create")) {
		CreateWeapon();
	}

	ImGui::DragFloat("ShotIntervalTime", &data_.shotIntervalTime, 0.01f, 0.0f);
	ImGui::DragFloat("BurstIntervalTime", &data_.burstIntervalTime, 0.01f, 0.0f);
	ImGui::DragFloat("BulletNum", &data_.bulletNum, 0.01f, 0.0f);
	ImGui::DragFloat("AttackValue", &data_.attackValue, 0.01f, 0.0f);
	ImGui::DragFloat("CoolTime", &data_.coolTime, 0.01f, 0.0f);
}

void WeaponUtility::CreateWeapon() {
	switch (selectWeapon_) {
	case (int)WeaponType::kMachineGun:
		
		break;
	case (int)WeaponType::kShotGun:
		
		break;
	case (int)WeaponType::kRifle:

		break;
	case (int)WeaponType::kLauncher:

		break;
	case (int)WeaponType::kMissile:

		break;
	case (int)WeaponType::kMelee:

		break;
	}
}

void WeaponUtility::ChangeWeapon() {

}

MachineGun* WeaponUtility::CreateMachineGun() {
	MachineGun* gun = new MachineGun(data_);
	return gun;
}

ShotGun* WeaponUtility::CreateShotGun() {
	ShotGun* gun = new ShotGun(data_);
	return gun;
}

Rifle* WeaponUtility::CreateRifle() {
	Rifle* gun = new Rifle(data_);
	return gun;
}

Launcher* WeaponUtility::CreateLauncher() {
	Launcher* gun = new Launcher(data_);
	return gun;
}

Missile* WeaponUtility::CreateMissile() {
	Missile* gun = new Missile(data_);
	return gun;
}

Melee* WeaponUtility::CreateMelee() {
	Melee* gun = new Melee(data_);
	return gun;
}
