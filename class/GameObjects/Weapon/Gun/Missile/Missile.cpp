#include "Missile.h"
#include "../../../Bullets/BulletManager.h"
#include "../../../Bullets/Bullet/Bullet.h"

using namespace FLMath;

Missile::Missile(WeaponData gunData)
	: IGun(gunData)
{
}

void Missile::Init() {
	IGun::Init();
}

void Missile::Update() {
	IGun::Update();
}

void Missile::Attack(int bulletHitFragBit, Actor* attackTarget) {
	bulletHitFragBit;
	attackTarget;
	//// 弾がない状態なら撃てない
	//if (magazine_->GetEmpty()) { 
	//	isDestroy_ = true;
	//	return; 
	//}
	//// 射撃できる状態か
	//if (!GetIsEnableShot()) { return; }

	//// 弾を撃つ
	//Bullet* bullet = new Bullet(body_.worldTF.GetWorldPosition(), GetDirVector({ 0,0,1 }, actor_->GetModel().worldTF.rotation));
	//pBulletManager_->CreateBullet(bullet);

	//// 弾数を減らす
	//magazine_->BulletDecrement();

	//// 射撃間隔を初期化
	//shotFrame_ = gunData_.shotIntervalTime * 60.0f;
}


void Missile::Destroy() {
	
}