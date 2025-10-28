#include "WeaponController.h"
#include "Action/None/NoneAction.h"
#include "../../Weapon/Gun/ShotGun/ShotGun.h"
#include "../../Weapon/Gun/MachineGun/MachineGun.h"
#include "../System/LeadingSystem.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace LWP::Input;

WeaponController::WeaponController(LeadingSystem* leadingSystem, Actor* target) {
	pLeadingSystem_ = leadingSystem;
	target_ = target;

	enableChangeState_ = Weapon::MainAction::reloading | Weapon::MainAction::attack;
	weapons_[WeaponSide::kLeft] = std::make_unique<WeaponSlot>(pLeadingSystem_);
	weapons_[WeaponSide::kRight] = std::make_unique<WeaponSlot>(pLeadingSystem_);
	weapons_[WeaponSide::kLeftShoulder] = std::make_unique<WeaponSlot>(pLeadingSystem_);
	weapons_[WeaponSide::kRightShoulder] = std::make_unique<WeaponSlot>(pLeadingSystem_);
}

WeaponController::~WeaponController() {}

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
		// 左肩
		if (ImGui::TreeNode("LeftShoulder")) {
			weapons_[WeaponSide::kLeftShoulder]->DebugGui();
			ImGui::TreePop();
		}
		// 右肩
		if (ImGui::TreeNode("RightShoulder")) {
			weapons_[WeaponSide::kRightShoulder]->DebugGui();
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void WeaponController::InputHandle() {
	// 武器装着箇所の指定を初期化する
	collectSide_ = WeaponSide::kCount;
	// 武器の回収ボタンを押しているか
	bool isCollect = false;
	if (Keyboard::GetPress(DIK_0) || Controller::GetPress(XBOX_X)) {
		isCollect = true;
	}

	// 左手
	if (Keyboard::GetPress(DIK_1) || Controller::GetPress(XBOX_LT)) {
		// 回収中なら武器装着要求を出す
		if (!weapons_[WeaponSide::kLeft]->GetIsFullWeapon() && isCollect) {
			collectSide_ = WeaponSide::kLeft;
		}
		else if(weapons_[WeaponSide::kLeft] && collectSide_ == WeaponSide::kCount){
			weapons_[WeaponSide::kLeft]->Attack();
		}
	}
	// 右手
	if (Keyboard::GetPress(DIK_2) || Controller::GetPress(XBOX_RT)) {
		// 回収中なら武器装着
		if (!weapons_[WeaponSide::kRight]->GetIsFullWeapon() && isCollect) {
			collectSide_ = WeaponSide::kRight;
		}
		else if (weapons_[WeaponSide::kRight] && collectSide_ == WeaponSide::kCount) {
			weapons_[WeaponSide::kRight]->Attack();
		}
	}
	// 左肩
	if (Keyboard::GetPress(DIK_3) || Controller::GetPress(XBOX_LB)) {
		// 回収中なら武器装着要求を出す
		if (!weapons_[WeaponSide::kLeftShoulder]->GetIsFullWeapon() && isCollect) {
			collectSide_ = WeaponSide::kLeftShoulder;
		}
		else if (weapons_[WeaponSide::kLeftShoulder] && collectSide_ == WeaponSide::kCount) {
			weapons_[WeaponSide::kLeftShoulder]->Attack();
		}
	}
	// 右肩
	if (Keyboard::GetPress(DIK_4) || Controller::GetPress(XBOX_RB)) {
		// 回収中なら武器装着
		if (!weapons_[WeaponSide::kRightShoulder]->GetIsFullWeapon() && isCollect) {
			collectSide_ = WeaponSide::kRightShoulder;
		}
		else if (weapons_[WeaponSide::kRightShoulder] && collectSide_ == WeaponSide::kCount) {
			weapons_[WeaponSide::kRightShoulder]->Attack();
		}
	}
}

void WeaponController::SetWeapon(IWeapon* weapon) {
	switch (collectSide_) {
	case WeaponSide::kLeft:
		SetLeftWeapon(weapon);
		break;
	case WeaponSide::kLeftShoulder:
		SetLeftShoulderWeapon(weapon);
		break;
	case WeaponSide::kRight:
		SetRightWeapon(weapon);
		break;
	case WeaponSide::kRightShoulder:
		SetRightShoulderWeapon(weapon);
		break;
	}
}