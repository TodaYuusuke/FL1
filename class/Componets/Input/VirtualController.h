#pragma once
#include "Device/IInputDevice.h"

class VirtualController final : public LWP::Utility::ISingleton<VirtualController> {
    friend class LWP::Utility::ISingleton<VirtualController>;// ISingletonをフレンドクラスにしてコンストラクタを呼び出せるように
private:
	VirtualController();
	~VirtualController() = default;

public:
	void Init();
	void Update();

public:// アクセサ

    bool PressAnyKey();
    bool TriggerAnyKey();
    /// <summary>
    /// キーが押されているかを取得
    /// </summary>
    /// <param name="code"></param>
    /// <returns></returns>
    bool GetPress(BindActionType actionType) const;
    /// <summary>
    /// キーが押された瞬間を取得
    /// </summary>
    /// <param name="code"></param>
    /// <returns></returns>
    bool GetTrigger(BindActionType actionType) const;
    /// <summary>
    /// キーが離された瞬間を取得
    /// </summary>
    /// <param name="code"></param>
    /// <returns></returns>
    bool GetRelease(BindActionType actionType) const;
    /// <summary>
    /// Lスティックの入力を取得
    /// </summary>
    /// <returns></returns>
    LWP::Math::Vector2 GetLAxis() const;
    /// <summary>
    /// Rスティックの入力を取得
    /// </summary>
    /// <returns></returns>
    LWP::Math::Vector2 GetRAxis() const;

private:
    std::vector<std::unique_ptr<IInputDevice>> device_;
};

