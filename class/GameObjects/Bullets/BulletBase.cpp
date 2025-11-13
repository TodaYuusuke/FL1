#include "BulletBase.h"

BulletBase::BulletBase(float attackPower, float speed) :
	bodyAABB_(bodyCollision_.SetBroadShape(LWP::Object::Collider::AABB()))
{
	attackPower_ = attackPower;
	moveSpeed_ = speed;
}

void BulletBase::OnCollision(LWP::Object::Collision* hitTarget) {
	hitTarget;
	isAlive_ = false;
}