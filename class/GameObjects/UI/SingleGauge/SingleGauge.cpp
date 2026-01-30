#include "SingleGauge.h"

void SingleGauge::Initialize(bool type) {
	type_ = type;
	backSprite_.material.color = {0.1f,0.1f,0.1f,1.0f};
	gaugeSprite_.material.color = { 166,238,175,255 };
	differenceSprite_.material.color = {0.8f,0.1f,0.1f,1.0f};
	backSprite_.anchorPoint = { 0.0f,0.0f };
	gaugeSprite_.anchorPoint = { 0.0f,0.0f };
	differenceSprite_.anchorPoint = { 0.0f,0.0f };
	differenceSprite_.isActive = false;
	gaugeSprite_.worldTF.Parent(&backSprite_.worldTF);
	differenceSprite_.worldTF.Parent(&backSprite_.worldTF);
	preRatio_ = 1.0f;
}

void SingleGauge::Update() {
	backSprite_.worldTF.translation = anchor_;
	backSprite_.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, LWP::Utility::DegreeToRadian(rotate_));
	backSprite_.worldTF.scale = size_;
	gaugeSprite_.worldTF.scale.x =  ratio_;
	
	if (type_) {
		differenceSprite_.isActive = true;

		if (preRatio_ > ratio_) {
			preRatio_ -= decay_;
		}
		if (preRatio_ < ratio_) {
			preRatio_ = ratio_;
		}
		differenceSprite_.worldTF.translation.x = differenceSprite_.GetFitSizeImpl(&differenceSprite_.material).x * ratio_;
		differenceSprite_.worldTF.scale.x = preRatio_ - ratio_;
	}

	backSprite_.worldTF.translation.z = 0.0f;
	differenceSprite_.worldTF.translation.z = -1.0f;
	gaugeSprite_.worldTF.translation.z = -2.0f;
}