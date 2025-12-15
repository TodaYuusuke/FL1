#include "CameraZoom.h"
#include "../../FollowCamera/FollowCamera.h"
#include "../../../../Componets/Math.h"
#include "../../CameraConfig.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace FLMath;

CameraZoom::CameraZoom(FollowCamera* camera, float zoomValue, float endTime)
	: CameraEffect(camera)
{
	Start(zoomValue, endTime);
}

void CameraZoom::Init() {

}

void CameraZoom::Update() {
	if (timer_) {
		zoomValue_ = Interp::LerpF(0, endZoom_, timer_->GetProgress());
		timer_->Update();
	}

	// 座標に適用
	float fov = Exponential(0.0f, zoomValue_, 0.2f);
	pCamera_->SetFov(kDefaultFov + fov);
}

void CameraZoom::Start(float zoomValue, float endTime) {
	if (timer_) { Finish(); }

	timer_ = std::make_unique<DeltaTimer>(endTime);
	timer_->Start();
	endZoom_ = zoomValue;
}

void CameraZoom::Finish() {
	timer_.reset();
	endZoom_ = 0.0f;
}
