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

void MachineGun::Destroy() {
	
}