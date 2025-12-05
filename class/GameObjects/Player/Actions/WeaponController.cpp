#include "WeaponController.h"
#include "Action/None/NoneAction.h"
#include "../../Weapon/Gun/ShotGun/ShotGun.h"
#include "../../Weapon/Gun/MachineGun/MachineGun.h"
#include "../System/LeadingSystem.h"

#include "../../../Componets/InputMyController/ControllerReceiver.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace LWP::Input;

WeaponController::WeaponController(LeadingSystem* leadingSystem, Actor* target) {
	vCon_ = VirtualController::GetInstance();
	pLeadingSystem_ = leadingSystem;
	target_ = target;

	// 各武器練度
	weaponSkills_ = std::make_unique<WeaponSkill>();

	enableChangeState_ = Weapon::MainAction::reloading | Weapon::MainAction::attack;
	weapons_[WeaponSide::kLeft] = std::make_unique<WeaponSlot>(pLeadingSystem_, weaponSkills_.get());
	weapons_[WeaponSide::kRight] = std::make_unique<WeaponSlot>(pLeadingSystem_, weaponSkills_.get());
	weapons_[WeaponSide::kLeftShoulder] = std::make_unique<WeaponSlot>(pLeadingSystem_, weaponSkills_.get());
	weapons_[WeaponSide::kRightShoulder] = std::make_unique<WeaponSlot>(pLeadingSystem_, weaponSkills_.get());

	Init();
}

WeaponController::~WeaponController() {}

void WeaponController::Init() {
	// 武器
	for (auto it = weapons_.begin(); it != weapons_.end(); it++) {
		if (it->second) it->second->Init();
	}

	// 管理クラスの調整項目
	json_.Init(kJsonDirectoryPath + "HasWeaponUI.json")
		.BeginGroup("LeftArm")
		.AddValue<LWP::Math::Vector3>("Scale"     , &sampleWeaponSurface_[WeaponSide::kLeft].worldTF.scale)
		.AddValue<LWP::Math::Quaternion>("Rotate" , &sampleWeaponSurface_[WeaponSide::kLeft].worldTF.rotation)
		.AddValue<LWP::Math::Vector3>("Translate" , &sampleWeaponSurface_[WeaponSide::kLeft].worldTF.translation)
		.EndGroup()
		.BeginGroup("RightArm")
		.AddValue<LWP::Math::Vector3>("Scale", &sampleWeaponSurface_[WeaponSide::kRight].worldTF.scale)
		.AddValue<LWP::Math::Quaternion>("Rotate", &sampleWeaponSurface_[WeaponSide::kRight].worldTF.rotation)
		.AddValue<LWP::Math::Vector3>("Translate", &sampleWeaponSurface_[WeaponSide::kRight].worldTF.translation)
		.EndGroup()
		.BeginGroup("LeftShoulder")
		.AddValue<LWP::Math::Vector3>("Scale", &sampleWeaponSurface_[WeaponSide::kLeftShoulder].worldTF.scale)
		.AddValue<LWP::Math::Quaternion>("Rotate", &sampleWeaponSurface_[WeaponSide::kLeftShoulder].worldTF.rotation)
		.AddValue<LWP::Math::Vector3>("Translate", &sampleWeaponSurface_[WeaponSide::kLeftShoulder].worldTF.translation)
		.EndGroup()
		.BeginGroup("RightShoulder")
		.AddValue<LWP::Math::Vector3>("Scale", &sampleWeaponSurface_[WeaponSide::kRightShoulder].worldTF.scale)
		.AddValue<LWP::Math::Quaternion>("Rotate", &sampleWeaponSurface_[WeaponSide::kRightShoulder].worldTF.rotation)
		.AddValue<LWP::Math::Vector3>("Translate", &sampleWeaponSurface_[WeaponSide::kRightShoulder].worldTF.translation)
		.EndGroup()
		.BeginGroup("Cockpit")
		.AddValue<LWP::Math::Vector3>("Scale", &cockpit_.worldTF.scale)
		.AddValue<LWP::Math::Quaternion>("Rotate", &cockpit_.worldTF.rotation)
		.AddValue<LWP::Math::Vector3>("Translate", &cockpit_.worldTF.translation)
		.EndGroup()

		.CheckJsonFile();


	//UI初期化
	for (int side = (int)WeaponSide::kLeft; side < (int)WeaponSide::kCount;side++) {
		for (int i = 0; i < (int)WeaponType::kCount; i++) {
			weaponSurfaces_[(WeaponSide)side][i].LoadTexture(WeaponConfig::TextureName::UI::uiName[i]);
			//weaponSurfaces_[(WeaponSide)side][i].material.color = { 1.0f,1.0f,1.0f,0.5f };
		}
		sampleWeaponSurface_[(WeaponSide)side].isActive = false;
	}

	//コックピット
	cockpit_.LoadShortPath("Cockpit/Cockpit.gltf");
}

void WeaponController::Update() {
	// 入力
	InputHandle();

	// 武器スロット
	for (auto it = weapons_.begin(); it != weapons_.end(); it++) {
		if (it->second) it->second->Update();
	}

	// 武器練度
	weaponSkills_->Update();

	//UI設定
	for (int side = (int)WeaponSide::kLeft; side < (int)WeaponSide::kCount; side++) {
		for (int i = 0; i < (int)WeaponType::kCount; i++) {
			weaponSurfaces_[(WeaponSide)side][i].worldTF.Parent(&cockpit_.worldTF);
			//weaponSurfaces_[(WeaponSide)side][i].LoadTexture(WeaponConfig::TextureName::UI::uiName[i]);
			weaponSurfaces_[(WeaponSide)side][i].isActive = false;
			weaponSurfaces_[(WeaponSide)side][i].worldTF.translation = sampleWeaponSurface_[(WeaponSide)side].GetCenterPosition();
			weaponSurfaces_[(WeaponSide)side][i].worldTF.rotation = sampleWeaponSurface_[(WeaponSide)side].worldTF.rotation;
			weaponSurfaces_[(WeaponSide)side][i].worldTF.scale = sampleWeaponSurface_[(WeaponSide)side].worldTF.scale;
			weaponSurfaces_[(WeaponSide)side][i].anchorPoint = {0.5f,0.5f};
		}
	}
	for (int side = (int)WeaponSide::kLeft; side < (int)WeaponSide::kCount; side++) {
		if (weapons_[(WeaponSide)side]->GetIsFullWeapon()) {
			auto type = WeaponConfig::GetWeaponType(weapons_[(WeaponSide)side]->GetFrontWeapon()->GetName());
			weaponSurfaces_[(WeaponSide)side][type].isActive = true;
			//weaponSurfaces_[(WeaponSide)side][type].worldTF.translation = LWP::Math::Matrix4x4::TransformCoord(weaponSurfaces_[(WeaponSide)side][type].worldTF.translation,debugOwner_->GetWorldTF()->GetAffineMatrix());
			//weaponSurfaces_[(WeaponSide)side][type].worldTF.rotation = LWP::Math::Quaternion::ConvertDirection(LWP::Math::Matrix4x4::TransformCoord(weaponSurfaces_[(WeaponSide)side][type].worldTF.rotation.vec(), debugOwner_->GetWorldTF()->GetAffineMatrix().Inverse()));
			//weaponSurfaces_[(WeaponSide)side][type].worldTF.translation;
		}
	}
	cockpit_.worldTF.Parent(debugOwner_->GetWorldTF());
}

void WeaponController::DebugGui() {
	// 武器を装備
	if (ImGui::TreeNode("Weapons")) {
		if (ImGui::TreeNode("Inventory")) {
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

		if (ImGui::TreeNode("Skill")) {
			weaponSkills_->DebugGui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("UI")) {
			json_.DebugGUI();
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
	if (vCon_->GetPress(BindActionType::kCollect)) {
		isCollect = true;
	}
	if (ControllerReceiver::GetInstance()->IsOpen() && Keyboard::GetPress(DIK_LSHIFT)) {
		isCollect =  true;
	}

	//マイコン入力取得
	bool lButton=false, lTrigger=false;
	bool rButton=false, rTrigger=false;

	if (ControllerReceiver::GetInstance()->IsOpen()) {
		lTrigger = ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickLeft.button0;
		lButton = ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickLeft.button1;

		rTrigger = ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickRight.button0;
		rButton = ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickRight.button1;
	}

	// 左手
	if (vCon_->GetPress(BindActionType::kLeftHand)) {
		// 回収中なら武器装着要求を出す
		if (!weapons_[WeaponSide::kLeft]->GetIsFullWeapon() && isCollect) {
			collectSide_ = WeaponSide::kLeft;
		}
		else if (weapons_[WeaponSide::kLeft] && collectSide_ == WeaponSide::kCount) {
			weapons_[WeaponSide::kLeft]->Attack();
		}
	}
	// 右手
	if (vCon_->GetPress(BindActionType::kRightHand)) {
		// 回収中なら武器装着
		if (!weapons_[WeaponSide::kRight]->GetIsFullWeapon() && isCollect) {
			collectSide_ = WeaponSide::kRight;
		}
		else if (weapons_[WeaponSide::kRight] && collectSide_ == WeaponSide::kCount) {
			weapons_[WeaponSide::kRight]->Attack();
		}
	}
	// 左肩
	if (vCon_->GetPress(BindActionType::kLeftShoulder)) {
		// 回収中なら武器装着要求を出す
		if (!weapons_[WeaponSide::kLeftShoulder]->GetIsFullWeapon() && isCollect) {
			collectSide_ = WeaponSide::kLeftShoulder;
		}
		else if (weapons_[WeaponSide::kLeftShoulder] && collectSide_ == WeaponSide::kCount) {
			weapons_[WeaponSide::kLeftShoulder]->Attack();
		}
	}
	// 右肩
	if (vCon_->GetPress(BindActionType::kRightShoulder)) {
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