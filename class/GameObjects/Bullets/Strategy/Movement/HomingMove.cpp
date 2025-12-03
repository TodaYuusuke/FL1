#include "HomingMove.h"
#include "StraightMove.h"
#include "../../BulletBase.h"

using namespace LWP;
using namespace LWP::Math;

HomingMove::HomingMove(Actor* target) {
	target_ = target;
	type_ = (int)MovementType::kHoming;
}

void HomingMove::Update(BulletBase* bullet) {
	// 追尾対象がいない
	if (target_ && !target_->GetIsAlive()) {
		target_ = nullptr;
		bullet->ChangeMovement(std::make_unique<StraightMove>());
		return;
	}

	// 座標変更
	Vector3 pos = bullet->GetWorldTF()->GetWorldPosition();
	// 速度加算
	pos += bullet->GetVelocity() * HitStopController::GetInstance()->GetDeltaTime();
	bullet->SetPos(pos);
}
