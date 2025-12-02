#pragma once
#include "../../AttackBase.h"

/// <summary>
/// 着弾時の応答基底クラス(着弾した瞬間のみ呼ばれる)
/// </summary>
class IImpact {
public:
    // コンストラクタ
    IImpact(ImpactData data) { data_ = data; }
    // デストラクタ
    virtual ~IImpact() = default;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="bullet"></param>
    virtual void Update(AttackBase* attack) { attack; }
    /// <summary>
    /// 衝突応答
    /// </summary>
    /// <param name="bullet"></param>
    virtual void OnHit(AttackBase* attack) = 0;

protected:
    // 調整情報
    ImpactData data_;
};