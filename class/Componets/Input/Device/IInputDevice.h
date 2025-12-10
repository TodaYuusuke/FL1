#pragma once
#include "../InputConfig.h"
#include <Adapter.h>

class IInputDevice {
public:
    // コンストラクタ
    IInputDevice() = default;
    // デストラクタ
    virtual ~IInputDevice() = default;

    /// <summary>
    /// 更新
    /// </summary>
    virtual void Update() = 0;

public:// アクセサ
    virtual bool PressAnyKey() = 0;
    virtual bool TriggerAnyKey() = 0;
    /// <summary>
    /// キーが押されているかを取得
    /// </summary>
    /// <param name="code"></param>
    /// <returns></returns>
    virtual bool GetPress(BindActionType code) const = 0;
    /// <summary>
    /// キーが押された瞬間を取得
    /// </summary>
    /// <param name="code"></param>
    /// <returns></returns>
    virtual bool GetTrigger(BindActionType code) const = 0;
    /// <summary>
    /// キーが離された瞬間を取得
    /// </summary>
    /// <param name="code"></param>
    /// <returns></returns>
    virtual bool GetRelease(BindActionType code) const = 0;
    /// <summary>
    /// Lスティックの入力を取得
    /// </summary>
    /// <returns></returns>
    virtual LWP::Math::Vector2 GetLAxis() = 0;
    /// <summary>
    /// Rスティックの入力を取得
    /// </summary>
    /// <returns></returns>
    virtual LWP::Math::Vector2 GetRAxis() = 0;

protected:
    std::array<int, (int)BindActionType::kCount> bindData_;
};