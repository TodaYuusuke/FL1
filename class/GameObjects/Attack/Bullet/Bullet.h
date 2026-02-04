#pragma once
#include "../BulletBase.h"

class Bullet : public BulletBase {
public:
	// コンストラクタ
	Bullet(const AttackData& data, Actor* target, IWeapon* weapon, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel);
	// デストラクタ
	~Bullet() = default;

private:

	// 弾道演出用平面
	LWP::Primitive::NormalStretchedBillboard trajectoryPlane_;

};