#pragma once
#include <Adapter.h>
#include "IWeapon.h"

class LeadingSystem;
/// <summary>
/// 片手の武器スロットを管理
/// </summary>
class WeaponSlot {
public:
    // コンス虎歌
    WeaponSlot(LeadingSystem* leadingSystem);
    // デストラクタ
    ~WeaponSlot() = default;

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

private:// 外部から受け取る変数
    // 偏差射撃計算機能
    LeadingSystem* pLeadingSystem_;

private:
    // 武器
    std::vector<std::unique_ptr<IWeapon>> weapons_;
    // 最大3個まで保持
    static constexpr size_t kMaxWeapons = 3; 
};