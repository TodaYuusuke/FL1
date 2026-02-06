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
	weapons_[WeaponSide::kLeft] = std::make_unique<WeaponSlot>(pLeadingSystem_, weaponSkills_.get(), target_, static_cast<int>(WeaponSide::kLeft));
	weapons_[WeaponSide::kRight] = std::make_unique<WeaponSlot>(pLeadingSystem_, weaponSkills_.get(), target_, static_cast<int>(WeaponSide::kRight));
	weapons_[WeaponSide::kLeftShoulder] = std::make_unique<WeaponSlot>(pLeadingSystem_, weaponSkills_.get(), target_, static_cast<int>(WeaponSide::kLeftShoulder));
	weapons_[WeaponSide::kRightShoulder] = std::make_unique<WeaponSlot>(pLeadingSystem_, weaponSkills_.get(), target_, static_cast<int>(WeaponSide::kRightShoulder));

	Init();
}

WeaponController::~WeaponController() {}

void WeaponController::Init() {
	// 武器
	for (auto it = weapons_.begin(); it != weapons_.end(); it++) {
		if (it->second) it->second->Init();
	}

	//コックピット
	cockpit_.LoadShortPath("Cockpit/Cockpit.gltf");

	colorSample_[size_t(RarityType::kCommon)] = { 170, 170, 170, 255 };
	colorSample_[size_t(RarityType::kUnCommon)] = { 56, 178, 65, 255 };
	colorSample_[size_t(RarityType::kRare)] = { 56, 56, 180, 255 };
	colorSample_[size_t(RarityType::kSuperRare)] = { 178, 56, 178, 255 };
	colorSample_[size_t(RarityType::kLegendary)] = { 178, 178, 20, 255 };
	colorSample_[size_t(RarityType::kOver)] = { 178, 20, 20, 255 };

	// 管理クラスの調整項目
	json_.Init(kJsonDirectoryPath + "HasWeaponUI.json")
		.BeginGroup("LeftArm")
		.AddValue<LWP::Math::Vector3>("Scale", &sampleWeaponSurface_[WeaponSide::kLeft].worldTF.scale)
		.AddValue<LWP::Math::Quaternion>("Rotate", &sampleWeaponSurface_[WeaponSide::kLeft].worldTF.rotation)
		.AddValue<LWP::Math::Vector3>("Translate", &sampleWeaponSurface_[WeaponSide::kLeft].worldTF.translation)
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
		.AddValue<LWP::Math::Vector3>("Translate", &cockpitTarget_)
		.EndGroup()
		.BeginGroup("NumBullet")
		.AddValue<LWP::Math::Vector3>("Scale", &sampleBulletSurface_.worldTF.scale)
		.AddValue<LWP::Math::Vector3>("Translate", &sampleBulletSurface_.worldTF.translation)
		.AddValue<int>("BulletNumDigit", &kBulletNumDigit_)
		.EndGroup()
		.BeginGroup("GaugeDistance")
		.AddValue<float>("Distance", &gaugeDistance_)
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
	for (int side = (int)WeaponSide::kLeft; side < (int)WeaponSide::kCount; side++) {
		for (int i = 0; i < (int)WeaponType::kCount; i++) {
			weaponSurfaces_[(WeaponSide)side][i].LoadTexture(WeaponConfig::TextureName::UI::uiName[i]);
			weaponGaugeSurfaces_[(WeaponSide)side][i].LoadTexture(WeaponConfig::TextureName::UIGauge::uiName[i]);
			weaponGaugeSurfaces_[(WeaponSide)side][i].clipRect.max = weaponTextureSize_;

			weaponCoolTimeSurfaces_[(WeaponSide)side][i].LoadTexture(WeaponConfig::TextureName::UI::uiName[i]);
			weaponCoolTimeSurfaces_[(WeaponSide)side][i].clipRect.max = weaponTextureSize_;
			
		}
		sampleWeaponSurface_[(WeaponSide)side].LoadTexture("Weapon/none_UI.png");
		sampleWeaponSurface_[(WeaponSide)side].isActive = false;
		sampleWeaponSurface_[(WeaponSide)side].anchorPoint = {0.0f,0.0f};
		raritySurface_[(WeaponSide)side].LoadTexture("Weapon/rarity_UI.png");
		raritySurface_[(WeaponSide)side].SetSplitSize(kRarityTextureSize_);
		raritySurface_[(WeaponSide)side].isActive = false;
		raritySurface_[(WeaponSide)side].anchorPoint = { 0.0f,0.0f };
		raritySurface_[(WeaponSide)side].worldTF.translation.z = -0.03f;
		raritySurface_[(WeaponSide)side].worldTF.Parent(&sampleWeaponSurface_[(WeaponSide)side].worldTF);

		//弾数表示
		bulletNums_[(WeaponSide)side].reset(new NumPlane);
		bulletNums_[(WeaponSide)side]->Initialize(kBulletNumDigit_);
		bulletNums_[(WeaponSide)side]->SetParent(&sampleWeaponSurface_[(WeaponSide)side].worldTF);
		sampleBulletSurface_.isActive = false;

		// 練度上昇パーティクルエミッタの登録
		powerUPEffectEmitters_[(WeaponSide)side] = EffectManager::GetInstance()->CreateNewEmitter("PowerUP", {0.0f, 0.0f, -0.1f}, true);
		switch ((WeaponSide)side)
		{
		case WeaponSide::kLeft:
			powerUPEffectEmitters_[(WeaponSide)side]->SetParent(&cockpit_, "LowerMonitor.L");
			break;
		case WeaponSide::kRight:
			powerUPEffectEmitters_[(WeaponSide)side]->SetParent(&cockpit_, "LowerMonitor.R");
			break;
		case WeaponSide::kLeftShoulder:
			powerUPEffectEmitters_[(WeaponSide)side]->SetParent(&cockpit_, "UpperMonitor.L");
			break;
		case WeaponSide::kRightShoulder:
			powerUPEffectEmitters_[(WeaponSide)side]->SetParent(&cockpit_, "UpperMonitor.R");
			break;
		}
		
		powerUPEffectEmitters_[(WeaponSide)side]->SetIsAutoEmit(false);
	}

	hpCircleSurface_.LoadTexture("HP_circle.png");
	hpCircleSurface_.worldTF.Parent(&cockpit_.worldTF);
	hpCircleSurface_.anchorPoint = { 0.5f,0.0f };
	hpCircleSurface_.clipRect.max = circleTextureSize_;
	hpCircleSurface_.material.color = { 56, 178, 65, 255 };
	//hpPlane_ = std::make_unique<NumPlane>();
	//hpPlane_->Initialize(3);
	//hpPlane_->SetParent(&cockpit_.worldTF);

	cockpitAnimationT_ = 0;
	isEndAnimation_=false;
}

void WeaponController::Update() {
	isPickUpWeapon_ = false;

	CockpitAnimation();
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
			weaponSurfaces_[(WeaponSide)side][i].anchorPoint = {0.0f,0.0f};
			weaponSurfaces_[(WeaponSide)side][i].material.color = colorSample_[side];

			weaponGaugeSurfaces_[(WeaponSide)side][i].worldTF.Parent(&sampleWeaponSurface_[(WeaponSide)side].worldTF);
			weaponGaugeSurfaces_[(WeaponSide)side][i].worldTF.translation = {0.0f,0.0f,0.0f};
			weaponGaugeSurfaces_[(WeaponSide)side][i].worldTF.translation.z = gaugeDistance_;
			weaponGaugeSurfaces_[(WeaponSide)side][i].isActive = false;
			weaponGaugeSurfaces_[(WeaponSide)side][i].anchorPoint = { 0.0f,0.0f };
			weaponGaugeSurfaces_[(WeaponSide)side][i].material.color = colorSample_[side];

			weaponCoolTimeSurfaces_[(WeaponSide)side][i].worldTF.Parent(&sampleWeaponSurface_[(WeaponSide)side].worldTF);
			weaponCoolTimeSurfaces_[(WeaponSide)side][i].worldTF.translation = { 0.0f,0.0f,0.0f };
			weaponCoolTimeSurfaces_[(WeaponSide)side][i].worldTF.translation.z = -0.01f;
			weaponCoolTimeSurfaces_[(WeaponSide)side][i].isActive = false;
			weaponCoolTimeSurfaces_[(WeaponSide)side][i].anchorPoint = { 0.0f,0.0f };
			
			raritySurface_[(WeaponSide)side].isActive = false;
		}
		sampleWeaponSurface_[(WeaponSide)side].isActive = false;
	}
	for (int side = (int)WeaponSide::kLeft; side < (int)WeaponSide::kCount; side++) {
		//武器を所有してるとき
		if (weapons_[(WeaponSide)side]->GetIsFullWeapon()) {
			auto type = weapons_[(WeaponSide)side]->GetFrontWeapon()->GetWeaponData().type;
			weaponSurfaces_[(WeaponSide)side][type].material.color = colorSample_[weapons_[(WeaponSide)side]->GetFrontWeapon()->GetWeaponData().rarity];
			weaponGaugeSurfaces_[(WeaponSide)side][type].material.color = colorSample_[weapons_[(WeaponSide)side]->GetFrontWeapon()->GetWeaponData().rarity];
			raritySurface_[(WeaponSide)side].material.color = colorSample_[weapons_[(WeaponSide)side]->GetFrontWeapon()->GetWeaponData().rarity];
			raritySurface_[(WeaponSide)side].index = weapons_[(WeaponSide)side]->GetFrontWeapon()->GetWeaponData().rarity;
			weaponSurfaces_[(WeaponSide)side][type].isActive = true;
			weaponGaugeSurfaces_[(WeaponSide)side][type].isActive = true;
			raritySurface_[(WeaponSide)side].isActive = true;
			CalcGauge(&weaponGaugeSurfaces_[(WeaponSide)side][type], weaponSkills_->GetSkillData(type).value);
			//クールタイムの表示
			if (!weapons_[(WeaponSide)side]->GetFrontWeapon()->GetIsEnableAttack()) {
				weaponCoolTimeSurfaces_[(WeaponSide)side][type].isActive = true;
				weaponCoolTimeSurfaces_[(WeaponSide)side][type].material.color = colorSample_[weapons_[(WeaponSide)side]->GetFrontWeapon()->GetWeaponData().rarity];
				weaponCoolTimeSurfaces_[(WeaponSide)side][type].material.color.R *= 0.4f;
				weaponCoolTimeSurfaces_[(WeaponSide)side][type].material.color.G *= 0.4f;
				weaponCoolTimeSurfaces_[(WeaponSide)side][type].material.color.B *= 0.4f;
				CalcCoolTime(&weaponCoolTimeSurfaces_[(WeaponSide)side][type], weapons_[(WeaponSide)side]->GetFrontWeapon()->GetEnableAttack(), weapons_[(WeaponSide)side]->GetFrontWeapon()->GetWeaponData().shotIntervalTime);
			}
			bulletNums_[(WeaponSide)side]->SetIsActive(true);
			bulletNums_[(WeaponSide)side]->SetNum(weapons_[(WeaponSide)side]->GetFrontWeapon()->GetBulletNum());
			bulletNums_[(WeaponSide)side]->SetColor(colorSample_[weapons_[(WeaponSide)side]->GetFrontWeapon()->GetWeaponData().rarity]);

			// 練度上昇時パーティクル生成
			if (weaponSkills_->GetSkillData(type).value > weaponSkills_->GetSkillData(type).preValue) {
				powerUPEffectEmitters_[(WeaponSide)side]->Emit();
			}
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

	// 練度の最終処理
	weaponSkills_->Finalize();
}

void WeaponController::CalcHP(Health* health) {
	float now = health->GetHealth();
	float max = health->GetMaxHealth();
	float ratio = now / max;
	hpCircleSurface_.anchorPoint.y = 1.0f - ratio;
	hpCircleSurface_.clipRect.min.y = (1.0f - ratio) * circleTextureSize_.y;
}

void WeaponController::CalcGauge(LWP::Primitive::ClipSurface* gauge, float value) {
	float now = value;
	float max = weaponSkills_->GetMaxWeaponSkill();
	if (max < now) {
		now = max;
	}
	float ratio = now / max;
	//ratio = 0.7f;
	gauge->anchorPoint.x =  0;
	gauge->clipRect.min = {0.0f,0.0f};
	gauge->clipRect.max.x = (ratio) * weaponTextureSize_.x;
}

void WeaponController::CalcCoolTime(LWP::Primitive::ClipSurface* gauge, float coolTime, float maxCoolTime) {
	float now = coolTime;
	float max = maxCoolTime*60.0f;
	if (max < now) {
		now = max;
	}
	float ratio = now / max;
	ratio = 1.0f - ratio;
	gauge->anchorPoint.y = 0;
	gauge->clipRect.min = { 0.0f,(ratio)*weaponTextureSize_.y };
	//gauge->clipRect.max.y = (ratio)*weaponTextureSize_.y;
}

void WeaponController::CockpitAnimation() {
	cockpit_.worldTF.translation = cockpitTarget_;

	float t = float(cockpitAnimationT_) / float(cockpitAnimationLength_);

	cockpit_.worldTF.translation.y = (1.0f - t)* cockpitOrigin_ + t* cockpitTarget_.y;

	cockpitAnimationT_++;
	if (cockpitAnimationT_ > cockpitAnimationLength_) {
		cockpitAnimationT_ = cockpitAnimationLength_;
		isEndAnimation_=true;
	}
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
		if (weapons_[WeaponSide::kLeft] && !weapons_[WeaponSide::kLeft]->GetIsEmptyWeapon()) {
			weapons_[WeaponSide::kLeft]->Attack();
		}
	}
	// 右手
	if (vCon_->GetPress(BindActionType::kRightHand)) {
		if (weapons_[WeaponSide::kRight] && !weapons_[WeaponSide::kRight]->GetIsEmptyWeapon()) {
			weapons_[WeaponSide::kRight]->Attack();
		}
	}
	// 左肩
	if (vCon_->GetPress(BindActionType::kLeftShoulder)) {
		if (weapons_[WeaponSide::kLeftShoulder] && !weapons_[WeaponSide::kLeftShoulder]->GetIsEmptyWeapon()) {
			weapons_[WeaponSide::kLeftShoulder]->Attack();
		}
	}
	// 右肩
	if (vCon_->GetPress(BindActionType::kRightShoulder)) {
		if (weapons_[WeaponSide::kRightShoulder] && !weapons_[WeaponSide::kRightShoulder]->GetIsEmptyWeapon()) {
			weapons_[WeaponSide::kRightShoulder]->Attack();
		}
	}

	// 回収中なら武器装着要求を出す
	if (!weapons_[WeaponSide::kLeft]->GetIsFullWeapon() && isCollect) {
		collectSide_ = WeaponSide::kLeft;
	}
	// 回収中なら武器装着要求を出す
	else if (!weapons_[WeaponSide::kLeftShoulder]->GetIsFullWeapon() && isCollect) {
		collectSide_ = WeaponSide::kLeftShoulder;
	}
	// 回収中なら武器装着要求を出す
	else if (!weapons_[WeaponSide::kRight]->GetIsFullWeapon() && isCollect) {
		collectSide_ = WeaponSide::kRight;
	}
	// 回収中なら武器装着要求を出す
	else if (!weapons_[WeaponSide::kRightShoulder]->GetIsFullWeapon() && isCollect) {
		collectSide_ = WeaponSide::kRightShoulder;
	}
}

void WeaponController::DeleteWeapons() {
	for (auto& [key, data] : weapons_) {
		data->DeleteWeapons();
	}
}

void WeaponController::SetWeapon(IWeapon* weapon) {
	switch (collectSide_) {
	case WeaponSide::kLeft:
		SetLeftWeapon(weapon);
		// プレイヤーに対して取得アニメーション再生を要求
		debugOwner_->PlayPickUpAnim(static_cast<int>(collectSide_));
		break;
	case WeaponSide::kLeftShoulder:
		SetLeftShoulderWeapon(weapon);
		// プレイヤーに対して取得アニメーション再生を要求
		debugOwner_->PlayPickUpAnim(static_cast<int>(collectSide_));
		break;
	case WeaponSide::kRight:
		SetRightWeapon(weapon);
		// プレイヤーに対して取得アニメーション再生を要求
		debugOwner_->PlayPickUpAnim(static_cast<int>(collectSide_));
		break;
	case WeaponSide::kRightShoulder:
		SetRightShoulderWeapon(weapon);
		// プレイヤーに対して取得アニメーション再生を要求
		debugOwner_->PlayPickUpAnim(static_cast<int>(collectSide_));
		break;
	}
}

void WeaponController::EmitPowerUPParticle(const WeaponSide side)
{
	// パーティクル生成
	powerUPEffectEmitters_[side]->Emit();
}
