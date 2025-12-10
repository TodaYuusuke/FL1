#include "BulletBase.h"
#include "../Collision/CollisionMask.h"
#include "AttackManager.h"
#include "Strategy/Impact/NormalImpact.h"
#include "Strategy/Impact/ExplosionImpact.h"
#include "Strategy/Movement/StraightMove.h"
#include "Strategy/Movement/HomingMove.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

BulletBase::BulletBase(const AttackData& data, Actor* target, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel)
	: AttackBase(hitFragBit),
	bodyAABB_(bodyCollision_.SetBroadShape<LWP::Object::Collider::AABB>())
{
	// 調整情報
	data_ = data;
	// 現在の攻撃力
	attackPower_ = data.attackValue;
	// 現在の速度
	moveSpeed_ = data.speed;
	// 生存時間
	currentFrame_ = data.elapsedTime * 60.0f;
	// 移動速度
	vel_ = dirVel;
	// 追尾対象
	target_ = target;

	// モデルの読み込み
	body_.LoadCube();
	// 座標
	body_.worldTF.translation = pos;
	// 大きさ
	body_.worldTF.scale = { 0.3f, 0.3f, 1.5f };
	// 角度
	body_.worldTF.rotation = Quaternion::LookRotation(dirVel);

	// 体の判定生成
	Vector3 size = data.attackSize / 2.0f;
	bodyAABB_.min = size * -1.0f;
	bodyAABB_.max = size;
	bodyCollision_.SetFollow(&body_.worldTF);
	bodyCollision_.isActive = true;
	// 自機の所属しているマスクを設定
	bodyCollision_.mask.SetBelongFrag(GameMask::attack);
	// 当たり判定をとる対象のマスクを設定
	hitFragBit_ = hitFragBit;
	bodyCollision_.mask.SetHitFrag(hitFragBit);
	bodyCollision_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		OnCollision(hitTarget);
		};

	// 着弾時の処理
	if (data_.impactType == (int)ImpactType::kExplosion) {
		impact_ = std::make_unique<ExplosionImpact>(AttackManager::GetInstance()->GetImpactData(data_.impactType));
	}

	if (data_.movementType == (int)MovementType::kStraight) {
		movement_ = std::make_unique<StraightMove>();
	}
	else if (data_.movementType == (int)MovementType::kHoming) {
		movement_ = std::make_unique<HomingMove>(target);
	}
}

void BulletBase::Init() {
	// 速度ベクトル
	vel_ = { 0.0f,0.0f,0.0f };
	// 倍率
	attackMultiply_ = 1.0f;
	speedMultiply_ = 1.0f;
	// 現在の攻撃力
	attackPower_ = data_.attackValue;
	// 現在の速度
	moveSpeed_ = data_.speed;
	// 生存時間
	currentFrame_ = data_.elapsedTime * 60.0f;
}

void BulletBase::Update() {
	attackPower_ = data_.attackValue * attackMultiply_;

	// 移動方式
	movement_->Update(this);

	// 地面についたら消える
	if (body_.worldTF.GetWorldPosition().y <= 0.0f) {
		if (isAlive_) {
			isAlive_ = false;
			if (impact_) impact_->OnHit(this);
		}
	}

	// 線損時間を過ぎているなら消す
	if (currentFrame_ <= 0.0f) {
		isAlive_ = false;
	}

	currentFrame_ -= stopController_->GetDeltaTime();
}


void BulletBase::OnCollision(LWP::Object::Collision* hitTarget) {
	if (isAlive_) {
		if (impact_) impact_->OnHit(this);
	}

	hitTarget;
	isAlive_ = false;
}