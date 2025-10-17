#include "WeaponController.h"
#include "Action/None/NoneAction.h"
#include "../../Weapon/Gun/ShotGun/ShotGun.h"
#include "../../Weapon/Gun/MachineGun/MachineGun.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace LWP::Input;

WeaponController::WeaponController() {
	enableChangeState_ = Weapon::MainAction::reloading | Weapon::MainAction::attack;
	weapons_[WeaponSide::kLeft] = std::make_unique<WeaponSlot>();
	weapons_[WeaponSide::kRight] = std::make_unique<WeaponSlot>();
}

WeaponController::~WeaponController() {
	// 武器
	for (auto it = weapons_.begin(); it != weapons_.end(); it++) {
		//delete it->second;
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
		// 左手
		if (ImGui::TreeNode("Left")) {
			weapons_[WeaponSide::kLeft]->DebugGui();
			ImGui::TreePop();
		}
		// 右手
		if (ImGui::TreeNode("Right")) {
			weapons_[WeaponSide::kRight]->DebugGui();
			ImGui::TreePop();
		}

		// 左の武器を装備
		if (ImGui::Button("SetLeftWeapon")) {
			GunData data = {
				"Gun/AR/AR.obj",
				"",
				0.1f,
				0.0f,
				0.0f,
				60.0f,
				10.0f,
				1.0f
			};
			std::unique_ptr<ShotGun> gun = std::make_unique<ShotGun>(data);
			gun->SetParent(debugOwner_);
			gun->SetTranslation(LWP::Math::Vector3{ -1.0f, -0.5f,2.0f });
			SetLeftWeapon(std::move(gun));
		}
		// 右の武器を装備
		if (ImGui::Button("SetRightWeapon")) {
			GunData data = {
				"Gun/ShotGun/Rifle.obj",
				"",
				0.1f,
				0.0f,
				0.0f,
				60.0f,
				10.0f,
				1.0f
			};
			std::unique_ptr<ShotGun> gun = std::make_unique<ShotGun>(data);
			gun->SetParent(debugOwner_);
			gun->SetTranslation(LWP::Math::Vector3{ 1.0f, -0.5f,2.0f });
			SetRightWeapon(std::move(gun));
		}

		ImGui::TreePop();
	}
}

void WeaponController::InputHandle() {
	// 左
	if (Keyboard::GetPress(DIK_1) || Controller::GetPress(XBOX_LB)) {
		// 他の武器や状態と並列しても問題ないかを判別
		//if () {
		if (weapons_[WeaponSide::kLeft]) weapons_[WeaponSide::kLeft]->Attack();
		//}
	}
	// 右
	if (Keyboard::GetPress(DIK_2) || Controller::GetPress(XBOX_RB)) {
		// 他の武器や状態と並列しても問題ないかを判別
		//if () {
		if (weapons_[WeaponSide::kRight]) weapons_[WeaponSide::kRight]->Attack();
		//}
	}
}