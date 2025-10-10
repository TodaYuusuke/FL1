#include "Bullet.h"

Bullet::Bullet(const LWP::Math::Vector3& pos, const LWP::Math::Vector3& dirVel) {
	// モデルの読み込み
	body_.LoadCube();

	// 座標
	body_.worldTF.translation = pos;
	// 大きさ
	body_.worldTF.scale = { 0.3f, 0.3f, 1.5f };
	// 角度
	body_.worldTF.rotation = LWP::Math::Quaternion::LookRotation(dirVel);

	// 移動速度
	vel_ = dirVel;
}

void Bullet::Init() {
	vel_ = { 0.0f,0.0f,0.0f };

	currentFrame_ = 60.0f;
}

void Bullet::Update() {
	// 座標変更
	body_.worldTF.translation += vel_;

	// 角度変更
	body_.worldTF.rotation = LWP::Math::Quaternion::LookRotation(vel_);

	// 線損時間を過ぎているなら消す
	if (currentFrame_ <= 0.0f) {
		isAlive_ = false;
	}

	currentFrame_--;
}