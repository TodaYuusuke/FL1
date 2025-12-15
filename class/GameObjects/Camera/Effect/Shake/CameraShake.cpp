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
	if (!shake_.GetIsActive()) { return; }

	// —h‚ê‚ÌXV
	shake_.Update();

	// —h‚ê‚Ì”ÍˆÍ‚ð‹·‚ß‚é
	shakeRange_ = Interp::Exponential(shakeRange_, Vector3{}, 0.1f);
	shake_.SetRange(shakeRange_);

	// ƒJƒƒ‰‚ÌŒü‚«‚É‰ž‚¶‚Ä—h‚ê‚é•ûŒü‚ð•Ï“®
	vel_ = shakeValue_ * Matrix4x4::CreateRotateXYZMatrix(pCamera_->GetCamera()->worldTF.rotation);

	// À•W‚É“K—p
	pCamera_->SetTranslation(pCamera_->GetCamera()->worldTF.GetWorldPosition() + vel_);
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
