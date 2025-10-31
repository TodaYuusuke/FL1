#include "ShotGun.h"
#include "../../../Bullets/BulletManager.h"
#include "../../../Bullets/Bullet/Bullet.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

ShotGun::ShotGun(WeaponData gunData)
	: IGun(gunData)
{
}

void ShotGun::Destroy() {

}