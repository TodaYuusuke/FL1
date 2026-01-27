#include "ShotGun.h"
#include "../../../Attack/AttackManager.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

ShotGun::ShotGun(WeaponData gunData)
	: IGun(gunData)
{
	// 射撃エフェクト生成
	attackEffectEmitter_ = EffectManager::GetInstance()->CreateNewEmitter("MuzzleFlash", { 0.0f, 0.0f, 0.0f }, true);
	// 射撃エフェクトを武器モデルのマズルに親子付け
	attackEffectEmitter_->SetParent(&body_, "Muzzle");
	// 粒子の自動生成OFF
	attackEffectEmitter_->SetIsAutoEmit(false);
}

void ShotGun::Destroy() {

}