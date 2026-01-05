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
		.CheckJsonFile();
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

	if (currentFrame_ <= 0.0f) {
		isTransitionWave_ = false;
		numberUI_->SetIsScoreDisplay(false);
		transitionSprite_.isActive = false;
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
}