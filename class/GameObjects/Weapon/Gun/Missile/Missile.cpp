#include "Missile.h"
#include "../../../Attack/AttackManager.h"

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

void Missile::Destroy() {
	
}