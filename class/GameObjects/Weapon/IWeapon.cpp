#include "IWeapon.h"

void IWeapon::FallingUpdate() {
	// 持ち主がいるなら処理しない
	if (actor_) { return; }
	// 地面に到達したら処理しない
	if (body_.GetJointWorldPosition("Muzzle").y <= 0.0f) { return; }
	
	// 重力
	velocity_.y -= 0.008f;
	// 前後左右の動きを徐々に遅くする
	velocity_.x *= 0.99f;
	velocity_.z *= 0.99f;

	body_.worldTF.translation += velocity_;
}
