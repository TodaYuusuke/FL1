#include "BaseTutorialData.h"
#include "../../../Audio/AudioPlayer.h"

using namespace LWP;
using namespace LWP::Math;

BaseTutorialData::BaseTutorialData() {
	successFont_.LoadTexture("UI/Tutorial/success_tutorial.png");
	successFont_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f, 200.0f, 0.0f };
	successFont_.material.color.A = 0;
	successFont_.isActive = false;

	flashingEffect_.SetAnimData(&flashAlpha_, 0,	255, 0.2f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 255,	255, 2.0f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 255,	100, 0.04f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 100,	255, 0.04f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 255,	100, 0.04f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 100,	255, 0.04f * 60.0f, LWP::Utility::Easing::OutExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 255,	0, 0.2f * 60.0f, LWP::Utility::Easing::InExpo);
	flashingEffect_.SetAnimData(&flashAlpha_, 0,	0, 1.0f * 60.0f, LWP::Utility::Easing::InExpo);
}

void BaseTutorialData::StartEffect() {
	if (!introFont_.isActive) {
		flashingEffect_.ResetData();		
		flashingEffect_.SetIsStart(true);

		// チュートリアル開始時音を鳴らす
		AudioPlayer::GetInstance()->PlayAudio("PopUp_SE.mp3", 1.0f, LWP::AudioConfig::Other);
	}

	introFont_.isActive = true;
	nextGuideInterval_++;

	flashingEffect_.Update();

	introFont_.material.color.A = static_cast<unsigned int>(flashAlpha_);

	if (flashingEffect_.GetIsEnd()) {
		isNextGuide_ = true;
		flashingEffect_.ResetData();
	}
}

void BaseTutorialData::FinishEffect() {
	if (!successFont_.isActive) { 
		flashingEffect_.ResetData();
		flashingEffect_.SetIsStart(true);
	}
	successFont_.isActive = true;
	nextGuideInterval_++;

	flashingEffect_.Update();

	successFont_.material.color.A = static_cast<unsigned int>(flashAlpha_);

	if (flashingEffect_.GetIsEnd()) {
		isFinish_ = true;
		flashingEffect_.ResetData();
	}
}

bool BaseTutorialData::NextGuide(float nextGuideInterval) {
	if (!isNextGuide_) { return false; }

	nextGuideInterval_++;

	if (nextGuideInterval_ >= nextGuideInterval) {
		nextGuideInterval_ = 0.0f;
		return true;
	}

	return false;
}
