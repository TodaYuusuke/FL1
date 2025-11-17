#include "BulletBase.h"

BulletBase::BulletBase(float attackPower, float speed, float elapsedTime)
	: bodyAABB_(bodyCollision_.SetBroadShape<LWP::Object::Collider::AABB>())
{
	attackPower_ = attackPower;
	moveSpeed_ = speed;
	currentFrame_ = elapsedTime * 60.0f;
}

void BulletBase::OnCollision(LWP::Object::Collision* hitTarget) {
	hitTarget;
	isAlive_ = false;
}