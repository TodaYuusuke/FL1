#include "WeaponController.h"
#include "Action/None/NoneAction.h"
#include "../../Weapon/Gun/Gun.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace LWP::Input;

WeaponController::WeaponController() {
	enableChangeState_ = Weapon::MainAction::reloading | Weapon::MainAction::attack;
}

WeaponController::~WeaponController() {
	// 武器
	for (auto it = weapons_.begin(); it != weapons_.end(); it++) {
		delete it->second;
	}
}

void WeaponController::Init() {
	// 武器
	for (auto it = weapons_.begin(); it != weapons_.end(); it++) {
		if (it->second) it->second->Init();
	}
}

void WeaponController::Update() {
	// 入力
	InputHandle();

	// 武器
	for (auto it = weapons_.begin(); it != weapons_.end(); it++) {
		if (it->second) it->second->Update();
	}
}

void WeaponController::DebugGui() {
	// 武器を装備
	if (ImGui::TreeNode("Weapons")) {
		// 左の武器を装備
		if (ImGui::Button("SetLeftWeapon")) {
			GunData data = {
				"",
				"",
				0.1f,
				0.0f,
				0.0f,
				120.0f,
				10.0f,
				1.0f
			};
			Gun* gun = new Gun(data);
			gun->SetParent(debugOwner_);
			SetLeftWeapon(gun);
		}
		// 右の武器を装備
		if (ImGui::Button("SetRightWeapon")) {
			GunData data = {
				"",
				"",
				0.1f,
				0.0f,
				0.0f,
				120.0f,
				10.0f,
				1.0f
			};
			Gun* gun = new Gun(data);
			gun->SetParent(debugOwner_);
			SetRightWeapon(gun);
		}

		ImGui::TreePop();
	}
}

void WeaponController::InputHandle() {
	// 左
	if (Keyboard::GetPress(DIK_1)) {
		// 他の武器や状態と並列しても問題ないかを判別
		//if () {
		if (weapons_[WeaponSide::kLeft]) weapons_[WeaponSide::kLeft]->Attack();
		//}
	}
	// 右
	if (Keyboard::GetPress(DIK_2)) {
		// 他の武器や状態と並列しても問題ないかを判別
		//if () {
		if (weapons_[WeaponSide::kRight]) weapons_[WeaponSide::kRight]->Attack();
		//}
	}
}