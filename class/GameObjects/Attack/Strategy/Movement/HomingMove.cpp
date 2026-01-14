#include "HomingMove.h"
#include "StraightMove.h"
#include "../../BulletBase.h"

using namespace LWP;
using namespace LWP::Math;

HomingMove::HomingMove(Actor* target) {
	target;
	//target_ = target;
	type_ = (int)MovementType::kHoming;
}

void HomingMove::Update(BulletBase* bullet) {
	// 追尾対象がいない
	if (!bullet->GetShooter() || !bullet->GetShooter()->GetIsAlive()) {
		//target_ = nullptr;
		bullet->ChangeMovement(std::make_unique<StraightMove>());
		return;
	}

	// 自機座標より少し上をホーミング
	LWP::Math::Vector3 playerPos = bullet->GetShooter()->GetModel().GetJointWorldPosition("LockOnAnchor");
	// 弾から自機への方向ベクトルを算出
	LWP::Math::Vector3 toPlayer = playerPos - bullet->GetWorldTF()->GetWorldPosition();
	// 正規化
	toPlayer = toPlayer.Normalize();

	// 球面線形保管により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	bullet->SetVelocity(LWP::Math::Vector3::Slerp(bullet->GetVelocity().Normalize(), toPlayer, bullet->GetAttackData().homingAccuracy) * bullet->GetAttackData().speed);

	// 座標変更
	Vector3 pos = bullet->GetWorldTF()->GetWorldPosition();
	// 速度加算
	pos += bullet->GetVelocity() * HitStopController::GetInstance()->GetDeltaTime();
	bullet->SetPos(pos);
}
