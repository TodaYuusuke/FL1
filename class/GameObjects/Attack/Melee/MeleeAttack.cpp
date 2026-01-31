#include "MeleeAttack.h"
#include "../../Collision/CollisionMask.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

MeleeAttack::MeleeAttack(const AttackData& data, LWP::Object::TransformQuat* target, IWeapon* weapon, int hitFragBit)
	: BulletBase(data, nullptr, weapon, Vector3{ 0,0,0 }, hitFragBit)
{
	targetTF_ = target;

	bodyCollision_.SetFollow(targetTF_);
	bodyCollision_.worldTF.translation = { 0.0f,0.0f,1.0f };
	bodyCollision_.isActive = true;
	// 所属しているマスクを設定
	bodyCollision_.mask.SetBelongFrag(GameMask::attack);
	// 当たり判定をとる対象のマスクを設定
	bodyCollision_.mask.SetHitFrag(hitFragBit);
	bodyCollision_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		OnCollision(hitTarget);
		};
}

void MeleeAttack::Init() {
	vel_ = { 0.0f,0.0f,0.0f };
}

void MeleeAttack::Update() {
	// 座標変更
	body_.worldTF.translation += vel_ * moveSpeed_ * stopController_->GetDeltaTime();

	// 攻撃している人がいないならペアレントを消す
	if (!targetTF_) bodyCollision_.SetFollow(nullptr);

	// 線損時間を過ぎているなら消す
	if (currentFrame_ <= 0.0f) {
		isAlive_ = false;
	}

	currentFrame_ -= stopController_->GetDeltaTime();
}