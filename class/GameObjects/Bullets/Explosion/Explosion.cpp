#include "Explosion.h"
#include "../BulletBase.h"
#include "../../Collision/CollisionMask.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

Explosion::Explosion(const BulletData& data, const Vector3& pos, int hitFragBit)
	: BulletBase(data, pos, hitFragBit)
{
}

void Explosion::Init() {

}

void Explosion::Update() {

}
