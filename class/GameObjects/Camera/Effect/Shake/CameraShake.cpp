#include "CameraShake.h"
#include "../../FollowCamera/FollowCamera.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;

CameraShake::CameraShake(FollowCamera* camera, LWP::Math::Vector3 range, float endTime)
	: CameraEffect(camera)
{
	Start(range, endTime);
}

void CameraShake::Init() {
	shake_.Init();
}

void CameraShake::Update() {
	if (!shake_.GetIsActive()) {
		isActive_ = false;
		return; 
	}

	// 揺れの更新
	shake_.Update();

	// 揺れの範囲を狭める
	shakeRange_ = Interp::Exponential(shakeRange_, Vector3{}, 0.1f);
	shake_.SetRange(shakeRange_);

	// カメラの向きに応じて揺れる方向を変動
	vel_ = shakeValue_/* * pCamera_->GetCamera()->worldTF.GetWorldRotateMatrix()*/;

	// 座標に適用
	pCamera_->SetTranslation(pCamera_->GetCamera()->worldTF.translation + vel_);
}

void CameraShake::Start(LWP::Math::Vector3 range, float endTime) {
	shake_.SetIsActive(true);
	shake_.SetEndFrame(endTime * 60.0f);
	shake_.SetRange(range);
	shakeRange_ = range;
	shake_.SetTarget(&shakeValue_);
}

void CameraShake::Finish() {
	shake_.Init();
}
