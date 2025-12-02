#pragma once
#include "IImpact.h"

/// <summary>
/// 着弾時に爆発
/// </summary>
class ExplosionImpact : public IImpact {
public:
    // コンストラクタ
    ExplosionImpact(ImpactData data);
    // デストラクタ
    ~ExplosionImpact() override = default;

    /// <summary>
    /// 着弾時の処理
    /// </summary>
    /// <param name="b"></param>
    void OnHit(AttackBase* attack) override;

private:
};
