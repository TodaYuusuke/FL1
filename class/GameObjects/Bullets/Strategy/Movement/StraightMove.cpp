#include "StraightMove.h"
#include "../../BulletBase.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

StraightMove::StraightMove() {
	type_ = (int)MovementType::kStraight;
}

void StraightMove::Update(BulletBase* b) {
	// 座標変更
	Vector3 pos = b->GetWorldTF()->GetWorldPosition();
	// 速度加算
	pos += b->GetVelocity() * HitStopController::GetInstance()->GetDeltaTime();
	b->SetPos(pos);

	// 角度変更
	b->SetRotation(LookRotationZLock(b->GetVelocity()));
}
