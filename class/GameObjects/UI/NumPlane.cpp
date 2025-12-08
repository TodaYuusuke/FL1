#include "NumPlane.h"

void NumPlane::Initialize(size_t num) {
	digit_ = num;
	score_ = 123;
	center_ = { 960.0f,540.0f };

	for (size_t i = digit_; i < kMaxNum_; i++) {
		surfaces_[i].anchorPoint = { 0.5f,0.5f };
		//surfaces_[i].worldTF.translation = { -1000.0f,-1000.0f,0.0f };
		surfaces_[i].isActive = false;
	}

	for (size_t i = 0; i < digit_; i++) {
		surfaces_[i].LoadTexture("Number.png");
		surfaces_[i].anchorPoint = { 0.5f,0.5f };
		//surfaces_[i].worldTF.translation = { center_.x,center_.y,0.0f };
		surfaces_[i].SetSplitSize(kTextureSize_);
		surfaces_[i].isActive = true;
	}
	SetNumbers();
	width_.x = kTextureSize_.x;
	CalcTranslate();
}

void NumPlane::SetIsActive(bool is) {
	for (size_t i = 0; i < digit_; i++) {
		surfaces_[i].isActive = is;
	}
}

void NumPlane::SetParent(LWP::Object::TransformQuat* parent) {
	for (size_t i = 0; i < kMaxNum_; i++) {
		surfaces_[i].worldTF.Parent(parent);
	}
}

void NumPlane::Update() {
	SetNumbers();
	CalcTranslate();
}

/*void NumPlane::SetClip(size_t digit, int num) {
	float min = float(num);
	float max = min + 1.0f;
	//surfaces_[digit].clipRect.min = { min / 10.0f,0.0f };
	//surfaces_[digit].clipRect.max = { max / 10.0f,1.0f };
}*/

void NumPlane::SetNumbers() {
	for (size_t index = 0; index < digit_; index++) {
		int drawScore = score_ / int(std::pow(10, digit_ - index - 1));
		drawScore %= 10;
		surfaces_[index].index = drawScore;
	}
}

void NumPlane::CalcTranslate() {
	int centerIndex = int(digit_) / 2;
	for (size_t i = 0; i < digit_; i++) {
		int offset = int(i) - centerIndex;
		width_.x = scale_.x* kDigitWidth_;
		surfaces_[i].worldTF.translation = { center_.x + float(offset) * width_.x,center_.y,center_.z };
		surfaces_[i].worldTF.scale = scale_;
	}
}