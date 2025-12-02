#pragma once
#include "IImpact.h"

/// <summary>
/// 通常
/// </summary>
class NormalImpact : public IImpact {
public:
    // コンストラクタ
    NormalImpact();
    // デストラクタ
    ~NormalImpact() override = default;

    /// <summary>
    /// 衝突応答の処理
    /// </summary>
    /// <param name="bullet"></param>
    void OnHit(AttackBase* attack) override { attack; }
};
