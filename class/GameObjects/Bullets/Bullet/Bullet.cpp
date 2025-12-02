#include "Bullet.h"
#include "../BulletBase.h"
#include "../../Collision/CollisionMask.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

Bullet::Bullet(const AttackData& data, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel)
	: BulletBase(data, pos, hitFragBit, dirVel)
{
}