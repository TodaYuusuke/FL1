#include "Explosion.h"
#include "../../Collision/CollisionMask.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

Explosion::Explosion(const ImpactData& data, const Vector3& pos, int hitFragBit)
	: AttackBase(hitFragBit),
	bodySphere_(bodyCollision_.SetBroadShape<LWP::Object::Collider::Sphere>())
{
	// 調整情報
	data_ = data;
	// 現在の攻撃力
	attackPower_ = data.attackValue;
	// 現在の速度
	moveSpeed_ = data.speed;
	// 生存時間
	currentFrame_ = data.elapsedTime * 60.0f;

	// モデルの読み込み
	body_.LoadFullPath("resources/model/Attack/Impact/explosion.obj");
	// 座標
	body_.worldTF.translation = pos;
	// 大きさ
	body_.worldTF.scale = { 10.0f, 10.0f, 10.0f };

	// 体の判定生成
	bodySphere_.radius = 10.0f;
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
}

void Explosion::Init() {

}

void Explosion::Update() {
	if (currentFrame_ <= 0.0f) { 
		isAlive_ = false;
		return;
	}

	currentFrame_ -= stopController_->GetDeltaTime();
}

void Explosion::OnCollision(LWP::Object::Collision* hitTarget) {
	hitTarget;
}