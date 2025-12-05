#include "ScoreUI.h"

void ScoreUI::Initialize(size_t num) {
	digit_ = num;
	score_ = 12345;
	center_ = { 960.0f,540.0f };
	for (size_t i = 0; i < digit_;i++) {
		sprites_[i].LoadTexture("Number.png");
		sprites_[i].anchorPoint = { 0.5f,0.5f };
		sprites_[i].worldTF.translation = { center_.x,center_.y,0.0f };
		sprites_[i].SetSplitSize(kTextureSize_);
	}
	for (size_t i = digit_; i < kMaxNum_;i++) {
		sprites_[i].anchorPoint = { 0.5f,0.5f };
		sprites_[i].worldTF.translation = { -1000.0f,-1000.0f,0.0f };
	}
	SetNumbers();
	width_.x = kTextureSize_.x;
	CalcTranslate();
}

void ScoreUI::Update() {
	ImGui::Begin("score");
	ImGui::DragFloat2("center",&(center_.x));
	ImGui::End();
	SetNumbers();
	CalcTranslate();
}

/*void ScoreUI::SetClip(size_t digit, int num) {
	float min = float(num);
	float max = min + 1.0f;
	//sprites_[digit].clipRect.min = { min / 10.0f,0.0f };
	//sprites_[digit].clipRect.max = { max / 10.0f,1.0f };
}*/

void ScoreUI::SetNumbers() {
	for (size_t index = 0; index < digit_; index++) {
		int drawScore = score_ / int(std::pow(10, digit_ - index - 1));
		drawScore %= 10;
		sprites_[index].index = drawScore;
	}
}

void ScoreUI::CalcTranslate() {
	int centerIndex = int(digit_) / 2;
	for (size_t i = 0; i < digit_; i++) {
		int offset = int(i) - centerIndex;
		sprites_[i].worldTF.translation = {center_.x + float(offset)*width_.x,center_.y,0.0f};
	}
}