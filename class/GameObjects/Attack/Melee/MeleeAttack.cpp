#include "MeleeAttack.h"
#include "../../Collision/CollisionMask.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

MeleeAttack::MeleeAttack(const AttackData& data, LWP::Object::TransformQuat* target, IWeapon* weapon, int hitFragBit)
	: BulletBase(data, nullptr, weapon, Vector3{ 0,0,0 }, hitFragBit)
{
	targetTF_ = target;

	bodyCapsule_.localOffset = Vector3{0,0,1} * target->GetRotateMatrix() * data.attackSize.z;
	bodyCapsule_.radius = data.attackSize.x;
	bodyCollision_.worldTF.translation = { 0.0f,0.0f,1.0f };
}

void MeleeAttack::Init() {
	vel_ = { 0.0f,0.0f,0.0f };
}

void MeleeAttack::Update() {
	// 座標変更
	body_.worldTF.translation += vel_ * moveSpeed_ * stopController_->GetDeltaTime();

	bodyCapsule_.localOffset = Vector3{ 0,0,1 } * targetTF_->GetRotateMatrix() * data_.attackSize.z;

	// 攻撃している人がいないならペアレントを消す
	if (!targetTF_) {
		bodyCollision_.SetFollow(nullptr);
	}
	else {
		body_.worldTF.translation = targetTF_->GetWorldPosition();
		//bodyCollision_.worldTF.translation = targetTF_->GetWorldPosition();
	}

	// 線損時間を過ぎているなら消す
	if (currentFrame_ <= 0.0f) {
		isAlive_ = false;
	}

	currentFrame_ -= stopController_->GetDeltaTime();
}