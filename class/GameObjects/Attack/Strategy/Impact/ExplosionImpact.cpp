#include "NormalImpact.h"
#include "ExplosionImpact.h"
#include "../../Explosion/Explosion.h"
#include "../../AttackManager.h"

ExplosionImpact::ExplosionImpact(ImpactData data)
	: IImpact(data)
{
}

void ExplosionImpact::OnHit(AttackBase* attack) {
	AttackManager::GetInstance()->CreateImpact((int)ImpactType::kExplosion, attack->GetWorldTF()->GetWorldPosition(), attack->GetHitFragBit(),attack->GetBelongFragBit());
}
