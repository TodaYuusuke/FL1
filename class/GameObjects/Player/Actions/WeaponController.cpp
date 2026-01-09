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

	colorSample_[0] = { 56, 178, 65, 255 };
	colorSample_[1] = { 56, 56, 180, 255 };
	colorSample_[2] = { 56, 178, 65, 255 };
	colorSample_[3] = { 56, 178, 65, 255 };

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
		.BeginGroup("NumBullet")
		.AddValue<LWP::Math::Vector3>("Scale", &sampleBulletSurface_.worldTF.scale)
		.AddValue<LWP::Math::Vector3>("Translate", &sampleBulletSurface_.worldTF.translation)
		.AddValue<int>("BulletNumDigit", &kBulletNumDigit_)
		.EndGroup()
		.BeginGroup("HPCircle")
		.AddValue<LWP::Math::Vector3>("Scale", &hpCircleSurface_.worldTF.scale)
		.AddValue<LWP::Math::Quaternion>("Rotate", &hpCircleSurface_.worldTF.rotation)
		.AddValue<LWP::Math::Vector3>("Translate", &hpCircleSurface_.worldTF.translation)
		.EndGroup()
		.BeginGroup("HPNum")
		.AddValue<LWP::Math::Vector3>("Scale", &transformHpPlane_.scale)
		.AddValue<LWP::Math::Quaternion>("Rotate", &transformHpPlane_.rotation)
		.AddValue<LWP::Math::Vector3>("Translate", &transformHpPlane_.translation)
		.EndGroup()


		.CheckJsonFile();


	//UI初期化
	for (int side = (int)WeaponSide::kLeft; side < (int)WeaponSide::kCount;side++) {
		for (int i = 0; i < (int)WeaponType::kCount; i++) {
			weaponSurfaces_[(WeaponSide)side][i].LoadTexture(WeaponConfig::TextureName::UI::uiName[i]);
			//weaponSurfaces_[(WeaponSide)side][i].material.color = { 1.0f,1.0f,1.0f,0.5f };
		}
		sampleWeaponSurface_[(WeaponSide)side].LoadTexture("Weapon/none_UI.png");
		sampleWeaponSurface_[(WeaponSide)side].isActive = false;

		//弾数表示
		bulletNums_[(WeaponSide)side].reset(new NumPlane);
		bulletNums_[(WeaponSide)side]->Initialize(kBulletNumDigit_);
		bulletNums_[(WeaponSide)side]->SetParent(&sampleWeaponSurface_[(WeaponSide)side].worldTF);
		sampleBulletSurface_.isActive = false;
	}

	//コックピット
	cockpit_.LoadShortPath("Cockpit/Cockpit.gltf");

	hpCircleSurface_.LoadTexture("HP_circle.png");
	hpCircleSurface_.worldTF.Parent(&cockpit_.worldTF);
	hpCircleSurface_.anchorPoint = {0.5f,0.0f};
	hpCircleSurface_.clipRect.max = circleTextureSize_;
	hpCircleSurface_.material.color = { 56, 178, 65, 255 };
	hpPlane_ = std::make_unique<NumPlane>();
	hpPlane_->Initialize(3);
	hpPlane_->SetParent(&cockpit_.worldTF);

	//radar_->Initialize();
	//radar_->SetParent(&cockpit_.worldTF);
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
			weaponSurfaces_[(WeaponSide)side][i].isActive = false;
			weaponSurfaces_[(WeaponSide)side][i].worldTF.translation = sampleWeaponSurface_[(WeaponSide)side].worldTF.translation;
			weaponSurfaces_[(WeaponSide)side][i].worldTF.rotation = sampleWeaponSurface_[(WeaponSide)side].worldTF.rotation;
			weaponSurfaces_[(WeaponSide)side][i].worldTF.scale = sampleWeaponSurface_[(WeaponSide)side].worldTF.scale;
			weaponSurfaces_[(WeaponSide)side][i].anchorPoint = {0.5f,0.5f};
			weaponSurfaces_[(WeaponSide)side][i].material.color = colorSample_[side];
		}
		sampleWeaponSurface_[(WeaponSide)side].isActive = false;
	}
	for (int side = (int)WeaponSide::kLeft; side < (int)WeaponSide::kCount; side++) {
		//武器を所有してるとき
		if (weapons_[(WeaponSide)side]->GetIsFullWeapon()) {
			auto type = WeaponConfig::GetWeaponType(weapons_[(WeaponSide)side]->GetFrontWeapon()->GetName());
			weaponSurfaces_[(WeaponSide)side][type].isActive = true;
			//weaponSurfaces_[(WeaponSide)side][type].material.color = { 56, 178, 65, 255 };
			bulletNums_[(WeaponSide)side]->SetIsActive(true);
			bulletNums_[(WeaponSide)side]->SetNum(weapons_[(WeaponSide)side]->GetFrontWeapon()->GetBulletNum());
		}
		else {
			sampleWeaponSurface_[(WeaponSide)side].worldTF.Parent(&cockpit_.worldTF);
			sampleWeaponSurface_[(WeaponSide)side].isActive = true;
			bulletNums_[(WeaponSide)side]->SetIsActive(false);
		}
	}
	cockpit_.worldTF.Parent(debugOwner_->GetWorldTF());

	//弾数
	for (int side = (int)WeaponSide::kLeft; side < (int)WeaponSide::kCount; side++) {
		bulletNums_[(WeaponSide)side]->SetCenter(sampleBulletSurface_.worldTF.translation);
		bulletNums_[(WeaponSide)side]->SetScale(sampleBulletSurface_.worldTF.scale);
		sampleWeaponSurface_[(WeaponSide)side].worldTF.Parent(&cockpit_.worldTF);
		bulletNums_[(WeaponSide)side]->SetParent(&(sampleWeaponSurface_[(WeaponSide)side].worldTF));
		bulletNums_[(WeaponSide)side]->Update();
	}
	//hpCircleSurface_.DebugGUI();
	//CalcHP();
	hpPlane_->SetCenter(transformHpPlane_.translation);
	hpPlane_->SetScale(transformHpPlane_.scale);
}

void WeaponController::CalcHP(Health* health) {
	float now = health->GetHealth();
	float max = health->GetMaxHealth();
	float ratio = now / max;
	hpCircleSurface_.anchorPoint.y = 1.0f - ratio;
	hpCircleSurface_.clipRect.min.y = (1.0f - ratio) * circleTextureSize_.y;
	hpPlane_->SetNum(int32_t(ratio*100.0f));
	hpPlane_->Update();
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