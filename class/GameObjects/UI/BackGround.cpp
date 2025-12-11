#include "BackGround.h"

void BackGround::Initialize() {
	sprite_.LoadTexture("out_background.png");
	sprite_.FitToTexture();
	sprite_.worldTF.scale = {2.0f,2.0f,1.0f};
	sprite_.anchorPoint = { 0.5f, 0.5f };
	sprite_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,  (LWP::Info::GetWindowHeightF() / 2.0f),1.0f };
}