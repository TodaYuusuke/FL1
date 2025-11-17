#include "MeleeAttack.h"
#include "../../Collision/CollisionMask.h"

using namespace FLMath;

MeleeAttack::MeleeAttack(const LWP::Math::Vector3& colliderSize, LWP::Object::TransformQuat* target, int hitFragBit, float attackPower, float elapsedTime)
	: BulletBase(attackPower, 0.0f, elapsedTime)
{
	stopController_ = HitStopController::GetInstance();
	target_ = target;

	// 体の判定生成
	LWP::Math::Vector3 size = colliderSize / 2.0f;
	bodyAABB_.min = size * -1.0f;
	bodyAABB_.max = size;

	bodyCollision_.SetFollow(target_);
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
	if (!target_) bodyCollision_.SetFollow(nullptr);

	// 線損時間を過ぎているなら消す
	if (currentFrame_ <= 0.0f) {
		isAlive_ = false;
	}

	currentFrame_ -= stopController_->GetDeltaTime();
}