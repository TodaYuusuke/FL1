#include "SceneChangeAnimation.h"

void DefaultSceneChangeAnimation::Initialize(const std::string& bgmPath)
{
	for (size_t i = 0; i < kSpriteNum_; i++) {
		sprites_[i].LoadTexture("sceneChanger.png");
		sprites_[i].anchorPoint = { 0.0f,0.0f };
		sprites_[i].worldTF.scale = { 2.0f,2.0f,1.0f};
		sprites_[i].SetSplitSize(kTextureSize_);
		sprites_[i].index = 7;
		sprites_[i].isActive = false;
	}
	sprites_[0].worldTF.translation = { 0.1f,0.1f,-0.5f };
	sprites_[1].worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,-0.1f,-0.5f };
	sprites_[2].worldTF.translation = { 0,  (LWP::Info::GetWindowHeightF() / 2.0f),-0.5f };
	sprites_[3].worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,  (LWP::Info::GetWindowHeightF() / 2.0f),-0.5f };
	sprites_[1].material.uvTransform.scale.x = -1.0f;
	sprites_[2].material.uvTransform.scale.y = -1.0f;

	sprites_[3].material.uvTransform.scale.x = -1.0f;
	sprites_[3].material.uvTransform.scale.y = -1.0f;
	sprites_[1].index=7 - sprites_[0].index;
	sprites_[3].index=7 - sprites_[0].index;

	animationLength_ = kSpritFrame_;
	type_ = 0;

	// BGM再生する
	bgmID_ = AudioPlayer::GetInstance()->PlayAudio(bgmPath, 0.35f, LWP::AudioConfig::BGM, true);
}

void DefaultSceneChangeAnimation::Start(int type) {
	type_ = type;
	sprites_[0].index = kSpritFrame_ * type_;
	frame_=0;
	rect_ = -1;
	if (!type)rect_ *= -1;
	isPlay_ = true;
	for (size_t i = 0; i < kSpriteNum_; i++) {
		sprites_[i].isActive = true;
	}

	// 終了時BGMの再生を停止
	if (type_ == 1) {
		// BGMの再生停止を指示
		if (Sound* sound = AudioPlayer::GetInstance()->GetAudioPlayer(bgmID_)) {
			sound->Stop(0.1f);
		}
	}
}

void DefaultSceneChangeAnimation::Update() {
	if (!isPlay_) return;
	if (frame_ >= animationLength_) {
		isPlay_ = false;
		if (rect_==1) {
			for (size_t i = 0; i < kSpriteNum_; i++) {
				sprites_[i].isActive = false;
			}
		}
		return;
	}
	sprites_[0].index = int(float(kSpritFrame_) * (float(frame_)/float(animationLength_)));
	if (type_) sprites_[0].index = kSpritFrame_ - sprites_[0].index -1;
	sprites_[1].index = kSpritFrame_ - sprites_[0].index;
	sprites_[2].index = sprites_[0].index;
	sprites_[3].index = kSpritFrame_ - sprites_[0].index;
	frame_++;
}