#include "Bullet.h"
#include "../BulletBase.h"
#include "../../Collision/CollisionMask.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

Bullet::Bullet(const AttackData& data, Actor* target, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel)
	: BulletBase(data, target, pos, hitFragBit, dirVel)
{
	// 弾道エフェクト生成
	bulletEffect_ = BulletEffector::GetInstance()->CreateBulletLineEffect({ 3.0f, 3.0f, 3.0f }, GetWorldTF(), &vel_);
}
