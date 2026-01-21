#include "WaveManager.h"
#include "../Componets/HitStopController.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Primitive;

WaveManager::WaveManager() {
	transitionSprite_.anchorPoint = { 0.5f, 0.5f };
	transitionSprite_.LoadTexture("wave_UI.png");
	transitionSprite_.isActive = false;

	// ウェーブ数
	numberUI_ = std::make_unique<ScoreUI>();
	numberUI_->Initialize(2);
	numberUI_->SetIsScoreDisplay(false);
	numberUI_->SetScore(0);

	centerPos_["Font"] = { 0.0f,0.0f };
	centerPos_["Number"] = { 0.0f,0.0f };

	json_.Init(kDirectoryPath + "WaveFont.json");
	json_.BeginGroup("CenterPos")
		.AddValue<Vector2>("Screen", &screenCenterPos_)
		.AddValue<Vector2>("Font", &centerPos_["Font"])
		.AddValue<Vector2>("Number", &centerPos_["Number"])
		.EndGroup()

		.BeginGroup("DropPercents")
		.BeginGroup("Wave110")
		.AddValue<int>("Common", &weaponRarityPercent_[0][RarityType::kCommon])
		.AddValue<int>("UnCommon", &weaponRarityPercent_[0][RarityType::kUnCommon])
		.AddValue<int>("Rare", &weaponRarityPercent_[0][RarityType::kRare])
		.AddValue<int>("SuperRare", &weaponRarityPercent_[0][RarityType::kSuperRare])
		.AddValue<int>("Legendary", &weaponRarityPercent_[0][RarityType::kLegendary])
		.EndGroup()

		.BeginGroup("Wave1120")
		.AddValue<int>("Common", &weaponRarityPercent_[1][RarityType::kCommon])
		.AddValue<int>("UnCommon", &weaponRarityPercent_[1][RarityType::kUnCommon])
		.AddValue<int>("Rare", &weaponRarityPercent_[1][RarityType::kRare])
		.AddValue<int>("SuperRare", &weaponRarityPercent_[1][RarityType::kSuperRare])
		.AddValue<int>("Legendary", &weaponRarityPercent_[1][RarityType::kLegendary])
		.EndGroup()

		.BeginGroup("Wave2130")
		.AddValue<int>("Common", &weaponRarityPercent_[2][RarityType::kCommon])
		.AddValue<int>("UnCommon", &weaponRarityPercent_[2][RarityType::kUnCommon])
		.AddValue<int>("Rare", &weaponRarityPercent_[2][RarityType::kRare])
		.AddValue<int>("SuperRare", &weaponRarityPercent_[2][RarityType::kSuperRare])
		.AddValue<int>("Legendary", &weaponRarityPercent_[2][RarityType::kLegendary])
		.EndGroup()

		.BeginGroup("Wave3140")
		.AddValue<int>("Common", &weaponRarityPercent_[3][RarityType::kCommon])
		.AddValue<int>("UnCommon", &weaponRarityPercent_[3][RarityType::kUnCommon])
		.AddValue<int>("Rare", &weaponRarityPercent_[3][RarityType::kRare])
		.AddValue<int>("SuperRare", &weaponRarityPercent_[3][RarityType::kSuperRare])
		.AddValue<int>("Legendary", &weaponRarityPercent_[3][RarityType::kLegendary])
		.EndGroup()

		.BeginGroup("Wave41")
		.AddValue<int>("Common", &weaponRarityPercent_[4][RarityType::kCommon])
		.AddValue<int>("UnCommon", &weaponRarityPercent_[4][RarityType::kUnCommon])
		.AddValue<int>("Rare", &weaponRarityPercent_[4][RarityType::kRare])
		.AddValue<int>("SuperRare", &weaponRarityPercent_[4][RarityType::kSuperRare])
		.AddValue<int>("Legendary", &weaponRarityPercent_[4][RarityType::kLegendary])
		.EndGroup()
		.EndGroup()
		.CheckJsonFile();
	json_.Save();
	flashingEffect_.SetAnimData(&flashAlpha_, 0, 255, 0.2f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 255, 255, 1.0f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 255, 100, 0.04f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 100, 255, 0.04f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 255, 100, 0.04f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 100, 255, 0.04f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 255, 0, 0.2f * 60.0f, LWP::Utility::Easing::InExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 0, 0, 1.0f * 60.0f, LWP::Utility::Easing::InExpo);
}

WaveManager::~WaveManager() {

}

void WaveManager::Init() {
	// ウェーブ数の初期化
	numberUI_->SetScore(0);
	numberUI_->SetIsScoreDisplay(false);
	transitionSprite_.isActive = false;
	currentWaveNum_ = 0;
	isTransitionWave_ = false;
	flashingEffect_.ResetData();
}

void WaveManager::Update() {
	// 文字列の座標を更新
	Vector3 pos = { screenCenterPos_.x, screenCenterPos_.y, 0.0f };
	pos.x += centerPos_["Font"].x;
	pos.y += centerPos_["Font"].y;
	transitionSprite_.worldTF.translation = pos;
	numberUI_->SetCenter(screenCenterPos_ + centerPos_["Number"]);

	// ウェーブ遷移演出
	TransitionEffect();

	// ウェーブ数
	numberUI_->Update();

	flashingEffect_.Update();
}

void WaveManager::DebugGui() {
	if (ImGui::BeginTabItem("Wave")) {
		if (ImGui::Button("Save")) {
			json_.Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			json_.Load();
		}

		if (ImGui::TreeNode("CenterPos")) {
			ImGui::DragFloat2("Screen", &screenCenterPos_.x, 0.1f);
			ImGui::DragFloat2("Font", &centerPos_["Font"].x, 0.1f);
			ImGui::DragFloat2("Number", &centerPos_["Number"].x, 0.1f);
			ImGui::TreePop();
		}

		if (ImGui::Button("Start")) {
			StartNextWave();
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			Init();
		}
		ImGui::EndTabItem();
	}
}

void WaveManager::TransitionEffect() {
	if (!isTransitionWave_) { return; }

	for (size_t i = 0; i < 2; i++) {
		numberUI_->SetAlhpa(i, static_cast<unsigned char>(flashAlpha_));
	}
	transitionSprite_.material.color.A = static_cast<unsigned char>(flashAlpha_);

	if (currentFrame_ <= 0.0f) {
		isTransitionWave_ = false;
		numberUI_->SetIsScoreDisplay(false);
		transitionSprite_.isActive = false;

		flashingEffect_.ResetData();
		currentFrame_ = transitionEffectTime * 60.0f;
	}

	currentFrame_ -= HitStopController::GetInstance()->GetDeltaTime();
}

void WaveManager::StartNextWave() {
	isTransitionWave_ = true;
	currentFrame_ = transitionEffectTime * 60.0f;
	currentWaveNum_++;
	numberUI_->SetScore(currentWaveNum_);
	numberUI_->SetIsScoreDisplay(true);
	transitionSprite_.isActive = true;
	flashingEffect_.SetIsStart(true);

	if (currentWaveNum_ >= 0 && currentWaveNum_ <= 10) {
		currentWeaponRarityPercent_.clear();
		currentWeaponRarityPercent_ = weaponRarityPercent_[0];
	}
	else if (currentWaveNum_ >= 11 && currentWaveNum_ <= 20) {
		currentWeaponRarityPercent_.clear();
		currentWeaponRarityPercent_ = weaponRarityPercent_[1];
	}
	else if (currentWaveNum_ >= 21 && currentWaveNum_ <= 30) {
		currentWeaponRarityPercent_.clear();
		currentWeaponRarityPercent_ = weaponRarityPercent_[2];
	}
	else if (currentWaveNum_ >= 31 && currentWaveNum_ <= 40) {
		currentWeaponRarityPercent_.clear();
		currentWeaponRarityPercent_ = weaponRarityPercent_[3];
	}
	else if (currentWaveNum_ >= 41) {
		currentWeaponRarityPercent_.clear();
		currentWeaponRarityPercent_ = weaponRarityPercent_[4];
	}
}