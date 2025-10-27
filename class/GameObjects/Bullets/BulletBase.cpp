#include "BulletBase.h"

BulletBase::BulletBase() :
	bodyAABB_(bodyCollision_.SetBroadShape(LWP::Object::Collider::AABB()))
{

}

void BulletBase::OnCollision(LWP::Object::Collision* hitTarget) {
	hitTarget;
	isAlive_ = false;
}