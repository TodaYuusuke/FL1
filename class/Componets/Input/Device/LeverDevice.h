#pragma once
#include "IInputDevice.h"
#include "../../InputMyController/ControllerReceiver.h"

class LeverDevice : public IInputDevice {
public:
    // コンストラクタ
    LeverDevice();
    // デストラクタ
    ~LeverDevice() override = default;

    /// <summary>
    /// 更新
    /// </summary>
    void Update()  override;

public:// アクセサ
    /// <summary>
    /// キーが押されているかを取得
    /// </summary>
    /// <param name="code"></param>
    /// <returns></returns>
    bool GetPress(BindActionType code) const override;
    /// <summary>
    /// キーが押された瞬間を取得
    /// </summary>
    /// <param name="code"></param>
    /// <returns></returns>
    bool GetTrigger(BindActionType code) const override;
    /// <summary>
    /// キーが離された瞬間を取得
    /// </summary>
    /// <param name="code"></param>
    /// <returns></returns>
    bool GetRelease(BindActionType code) const override;
    /// <summary>
    /// Lスティックの入力を取得
    /// </summary>
    /// <returns></returns>
    LWP::Math::Vector2 GetLAxis() override;
    /// <summary>
    /// Rスティックの入力を取得
    /// </summary>
    /// <returns></returns>
    LWP::Math::Vector2 GetRAxis()  override;

private:
    GameUsedData::multiSticks::stick2Shaft preKeys_;
};