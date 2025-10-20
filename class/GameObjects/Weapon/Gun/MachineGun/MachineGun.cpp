#include "MachineGun.h"
#include "../../../Bullets/BulletManager.h"
#include "../../../Bullets/Bullet/Bullet.h"

using namespace FLMath;

MachineGun::MachineGun(WeaponData gunData)
	: IGun(gunData)
{
}

void MachineGun::Init() {
	IGun::Init();
}

void MachineGun::Update() {
	IGun::Update();
}

//void MachineGun::Attack() {
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
//}


void MachineGun::Destroy() {
	
}