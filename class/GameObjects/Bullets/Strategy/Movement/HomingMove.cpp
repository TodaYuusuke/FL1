#include "HomingMove.h"

using namespace LWP;
using namespace LWP::Math;

HomingMove::HomingMove(Actor* target) {
	target_ = target;
}

void HomingMove::Update(BulletBase* b) {
	// 座標変更
	Vector3 pos = b->GetWorldTF()->GetWorldPosition();
	// 速度加算
	pos += b->GetVelocity() * HitStopController::GetInstance()->GetDeltaTime();
	b->SetPos(pos);
}
