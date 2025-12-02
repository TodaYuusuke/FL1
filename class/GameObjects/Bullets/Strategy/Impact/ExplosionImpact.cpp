#include "NormalImpact.h"
#include "ExplosionImpact.h"
#include "../../Explosion/Explosion.h"
#include "../../BulletManager.h"

ExplosionImpact::ExplosionImpact(ImpactData data)
	: IImpact(data)
{
}

void ExplosionImpact::OnHit(AttackBase* attack) {
	BulletManager::GetInstance()->CreateImpact((int)ImpactType::kExplosion, attack->GetWorldTF()->GetWorldPosition(), attack->GetHitFragBit());
}
