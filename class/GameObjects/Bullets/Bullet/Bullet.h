#pragma once
#include "../BulletBase.h"

class Bullet : public BulletBase {
public:
	// コンストラクタ
	Bullet(const AttackData& data, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel);
	// デストラクタ
	~Bullet() = default;

private:
};