#pragma once
#include <Adapter.h>
#include "IWeapon.h"

/// <summary>
/// 片手の武器スロットを管理
/// </summary>
class WeaponSlot {
public:

    /// <summary>
    /// 初期化
    /// </summary>
    void Init();
    /// <summary>
    /// 更新
    /// </summary>
    void Update();
    /// <summary>
    /// 調整項目
    /// </summary>
    void DebugGui();

    /// <summary>
    /// 武器による攻撃
    /// </summary>
    void Attack();

private:
    /// <summary>
    /// リスト前詰め
    /// </summary>
    void Compact();

public:// アクセサ
    /// <summary>
    /// 武器の追加
    /// </summary>
    /// <param name="weapon"></param>
    void AddWeapon(std::unique_ptr<IWeapon> weapon);

private:
    std::vector<std::unique_ptr<IWeapon>> weapons_;
    // 最大3個まで保持
    static constexpr size_t kMaxWeapons = 3; 
};