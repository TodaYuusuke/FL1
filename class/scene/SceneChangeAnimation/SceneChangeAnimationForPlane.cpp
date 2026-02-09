#include "SceneChangeAnimationForPlane.h"

void SceneChangeAnimationPlane::Initialize(const std::string& bgmPath)
{
	for (size_t i = 0; i < kSpriteNum_; i++) {
		sprites_[i].LoadTexture("sceneChanger.png");
		//sprites_[i].anchorPoint = { 0.0f,0.0f };
		//sprites_[i].worldTF.scale = { 2.0f,2.0f,1.0f };
		sprites_[i].SetSplitSize(kTextureSize_);
		sprites_[i].index = 0;
		sprites_[i].isActive = true;
	}
	sprites_[0].worldTF.translation = { 0.0f,9.0f,1.0f };
	sprites_[1].worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,-0.1f,1.0f };
	sprites_[2].worldTF.translation = { 0,  (LWP::Info::GetWindowHeightF() / 2.0f),1.0f };
	sprites_[3].worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,  (LWP::Info::GetWindowHeightF() / 2.0f),1.0f };
	sprites_[1].material.uvTransform.scale.x = -1.0f;
	sprites_[2].material.uvTransform.scale.y = -1.0f;

	sprites_[3].material.uvTransform.scale.x = -1.0f;
	sprites_[3].material.uvTransform.scale.y = -1.0f;
	sprites_[1].index = 7 - sprites_[0].index;
	sprites_[3].index = 7 - sprites_[0].index;

	sprites_[0].isActive = true;
	sprites_[1].isActive = true;

	animationLength_ = kSpritFrame_;
	type_ = 0;


	// 管理クラスの調整項目
	json_.Init(kJsonDirectoryPath + "HasWeaponUI.json")
		.BeginGroup("LeftTop")
		.AddValue<LWP::Math::Vector3>("Scale", &sprites_[0].worldTF.scale)
		.AddValue<LWP::Math::Vector3>("Translate", &sprites_[0].worldTF.translation)
		.EndGroup()

		.BeginGroup("RightTop")
		.AddValue<LWP::Math::Vector3>("Scale", &sprites_[1].worldTF.scale)
		.AddValue<LWP::Math::Vector3>("Translate", &sprites_[1].worldTF.translation)
		.EndGroup()

		.BeginGroup("LeftBottom")
		.AddValue<LWP::Math::Vector3>("Scale", &sprites_[2].worldTF.scale)
		.AddValue<LWP::Math::Vector3>("Translate", &sprites_[2].worldTF.translation)
		.EndGroup()

		.BeginGroup("RightBottom")
		.AddValue<LWP::Math::Vector3>("Scale", &sprites_[3].worldTF.scale)
		.AddValue<LWP::Math::Vector3>("Translate", &sprites_[3].worldTF.translation)
		.EndGroup()


		.CheckJsonFile();

	// BGM再生する
	bgmID_ = AudioPlayer::GetInstance()->PlayAudio(bgmPath, 0.35f, LWP::AudioConfig::BGM, true);
}

void SceneChangeAnimationPlane::Start(int type) {
	type_ = type;
	sprites_[0].index = kSpritFrame_ * type_;
	frame_ = 0;
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

void SceneChangeAnimationPlane::Update() {
	if (!isPlay_) return;
	if (frame_ >= animationLength_) {
		isPlay_ = false;
		if (rect_ == 1) {
			for (size_t i = 0; i < kSpriteNum_; i++) {
				sprites_[i].isActive = false;
			}
		}
		
		return;
	}
	sprites_[0].index = int(float(kSpritFrame_) * (float(frame_) / float(animationLength_)));
	if (type_) sprites_[0].index = kSpritFrame_ - sprites_[0].index - 1;
	sprites_[1].index = kSpritFrame_ - sprites_[0].index;
	sprites_[2].index = sprites_[0].index;
	sprites_[3].index = kSpritFrame_ - sprites_[0].index;
	frame_++;
}

void SceneChangeAnimationPlane::SetParent(LWP::Object::TransformQuat* parent) {
	for (size_t i = 0; i < kSpriteNum_; i++) {
		sprites_[i].worldTF.Parent(parent);
	}
}

void SceneChangeAnimationPlane::DebugGui() {
	if (ImGui::BeginTabItem("SceneChange")) {
		json_.DebugGUI();
		ImGui::EndTabItem();
	}
}