#include "CameraBound.h"
#include "../../FollowCamera/FollowCamera.h"
#include "../../../../Componets/HitStopController.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;

CameraBound::CameraBound(FollowCamera* camera, LWP::Math::Vector3 moveValue, float endTime)
	: CameraEffect(camera)
{
	Start(moveValue, endTime);
}

CameraBound::~CameraBound() {
	Finish();
}

void CameraBound::Init() {

}

void CameraBound::Update() {
	if (currentFrame_ / endTime_ * 60.0f >= 1.0f) {
		isActive_ = false;
	}

	// 跳ねる挙動
	BoundMove();

	// カメラの向きに応じて揺れる方向を変動
	vel_ = boundValue_ * Matrix4x4::CreateRotateXYZMatrix(pCamera_->GetCamera()->worldTF.rotation);

	// 座標に適用
	pCamera_->SetTranslation(pCamera_->GetCamera()->worldTF.translation + vel_);

	currentFrame_ += HitStopController::GetInstance()->GetDeltaTime();
}

void CameraBound::Start(LWP::Math::Vector3 moveValue, float endTime) {
	endTime_ = endTime;
	endBoundValue_ = moveValue;
}

void CameraBound::Finish() {
	endBoundValue_ = {};
}

void CameraBound::BoundMove() {
	if (currentFrame_ / (endTime_ * 60.0f) > 1.0f) { return; }

	Vector3 start{};
	Vector3 end = endBoundValue_;
	float t = currentFrame_ / (endTime_ * 60.0f) * 2.0f;
	if (t >= 1.0f) {
		start = endBoundValue_;
		end = {};
		t -= 1.0f;
	}

	boundValue_ = Interp::Lerp(start, end, t);
}
