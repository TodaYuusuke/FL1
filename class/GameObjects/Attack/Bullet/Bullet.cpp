#include "Bullet.h"
#include "../BulletBase.h"
#include "../../Collision/CollisionMask.h"
#include "../../Weapon/IWeapon.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

Bullet::Bullet(const AttackData& data, Actor* target, IWeapon* weapon, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel)
	: BulletBase(data, target, weapon, pos, hitFragBit, dirVel)
{
	// エフェクト名称取得
	std::string effectName = weapon->GetWeaponData().attackEffectName;

	if (effectName == "MuzzleFlash") {
		// 弾道エフェクト生成
		bulletEffect_ = BulletEffector::GetInstance()->CreateBulletLineEffect({ 3.0f, 3.0f, 3.0f }, GetWorldTF(), &vel_);
	}

	if (effectName == "ShotSmoke") {
		// 弾道エフェクト生成
		bulletEffect_ = BulletEffector::GetInstance()->CreateExplosiveBulletEffect({ 3.0f, 3.0f, 3.0f }, GetWorldTF());
	}
}
