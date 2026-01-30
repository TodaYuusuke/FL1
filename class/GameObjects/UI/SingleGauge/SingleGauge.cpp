#include "SingleGauge.h"

void SingleGauge::Initialize(bool type) {
	type_ = type;
	//backSprite_.LoadTexture("../system/texture/white.png");
	backSprite_.material.color = {0.1f,0.1f,0.1f,1.0f};
	//gaugeSprite_.LoadTexture("../system/texture/white.png");
	gaugeSprite_.material.color = { 166,238,175,255 };
	backSprite_.anchorPoint = { 0.0f,0.0f };
	gaugeSprite_.anchorPoint = { 0.0f,0.0f };
	differenceSprite_.isActive = false;
}

void SingleGauge::Update() {
	backSprite_.worldTF.translation = anchor_;
	gaugeSprite_.worldTF.translation = anchor_;
	backSprite_.worldTF.scale = size_;
	gaugeSprite_.worldTF.scale.x = size_.x * ratio_;
	gaugeSprite_.worldTF.scale.y = size_.y;

	//backSprite_.;
}