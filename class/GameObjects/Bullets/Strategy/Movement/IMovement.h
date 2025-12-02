#pragma once
#include "../../BulletBase.h"

/// <summary>
/// 弾の移動方式基底クラス
/// </summary>
class IMovement {
public:
    // コンストラクタ
    IMovement() = default;
    // デストラクタ
    virtual ~IMovement() = default;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="bullet"></param>
    virtual void Update(BulletBase* bullet) = 0;
};