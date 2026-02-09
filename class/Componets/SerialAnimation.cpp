#include "SerialAnimation.h"

SerialAnimation::SerialAnimation(const std::string& spriteName, const LWP::Math::Vector2& splitSize) {
	sprite_.LoadTexture(spriteName);
	sprite_.anchorPoint = { 0.5f, 0.5f };
	sprite_.SetSplitSize(splitSize);
	sprite_.isActive = false;

	splitNum_ = sprite_.material.texture.t.GetSize().x / splitSize.x;
	splitNum_--;
}

void SerialAnimation::Init() {
	currentFrame_ = 0.0f;
	isStart_ = false;
	isEnd_ = false;
	isWaiting_ = false;
	sprite_.isActive = false;
}

void SerialAnimation::Update() {
	if (!isStart_) { return; }

	if (!isEnd_ && nextAnimInterval_ <= currentFrame_) {
		// 最後の連番画像まで再生した
		if (sprite_.index >= splitNum_) {
			isEnd_ = true;
		}
		else {
			sprite_.index++;
			currentFrame_ = 0.0f;
		}
	}

	// 待機時間終了後の処理
	if (isEnd_ && waitFrame_ <= currentFrame_) {
		// ループ
		if (isLoop_) {
			isEnd_ = false;
			currentFrame_ = 0.0f;
			sprite_.index = 0;
		}
		// 終了
		else {
			isStart_ = false;
			sprite_.isActive = false;
		}
		isWaiting_ = true;
	}
	currentFrame_++;
}

void SerialAnimation::DebugGui() {
	if (ImGui::TreeNode("Serial")) {
		sprite_.DebugGUI();
		ImGui::TreePop();
	}
}

void SerialAnimation::Start(float nextAnimInterval, float waitFrame, bool isLoop) {
	isLoop_ = isLoop;
	nextAnimInterval_ = nextAnimInterval;
	waitFrame_ = waitFrame;
	currentFrame_ = 0.0f;
	isStart_ = true;
	isWaiting_ = false;
	sprite_.isActive = true;
}

void SerialAnimation::ChangeSprite(const std::string& spriteName, const LWP::Math::Vector2& splitSize) {
	sprite_.LoadTexture(spriteName);
	sprite_.anchorPoint = { 0.5f, 0.5f };
	sprite_.SetSplitSize(splitSize);
	currentFrame_ = 0.0f;
	isEnd_ = false;
}