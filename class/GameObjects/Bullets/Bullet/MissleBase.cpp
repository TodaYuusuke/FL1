#include "MissleBase.h"
#include "../BulletBase.h"
#include "../BulletManager.h"
#include "../Explosion/Explosion.h"
#include "../../Collision/CollisionMask.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

MissleBase::MissleBase(const AttackData& data, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel)
	: BulletBase(data, pos, hitFragBit, dirVel)
{
}
//
//void MissleBase::OnCollision(LWP::Object::Collision* hitTarget) {
//	hitTarget;
//	isAlive_ = false;
//	// 当たったら爆発開始
//	if (isAlive_) {
//		//Explosion* attack = new Explosion(explosionSize_, body_.worldTF.GetWorldPosision(), GameMask::attack, )
//		//BulletManager::GetInstance()->CreateBullet()
//	}
//}