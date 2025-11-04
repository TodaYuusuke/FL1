#pragma once
#include "Adapter.h"

class HitStopController final : public LWP::Utility::ISingleton<HitStopController> {
	friend class LWP::Utility::ISingleton<HitStopController>;// ISingletonをフレンドクラスにしてコンストラクタを呼び出せるように{
public:
	// コンストラクタ
	HitStopController();
	// デストラクタ
	~HitStopController() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

public:// アクセサ
	/// <summary>
	/// ヒットストップ開始
	/// </summary>
	/// <param name="time"></param>
	/// <param name="multiply"></param>
	void Start(const float& time, const float& multiply = 0.2f) {
		SetHitStopTime(time);
		SetTimeMultiply(multiply);
		isFinish_ = false;
	}
#pragma region Getter
	/// <summary>
	/// フレームレートの取得
	/// </summary>
	/// <returns></returns>
	float GetFPS() { return fps; }
	/// <summary>
	/// ヒットストップ時間を取得
	/// </summary>
	float GetHitStopTime() { return hitStopTime_; }
	/// <summary>
	/// ヒットストップ時間を取得
	/// </summary>
	float GetCurrentFrame() { return currentFrame_; }
	/// <summary>
	/// 経過時間を取得
	/// </summary>
	/// <returns></returns>
	float GetDeltaTime() { return LWP::Info::GetDeltaTimeF() * fps; }
	/// <summary>
	/// ヒットストップが終了しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsFinish() { return isFinish_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// ヒットストップ時間を設定
	/// </summary>
	/// <param name="time">秒数</param>
	void SetHitStopTime(const float& time) {
		hitStopTime_ = time * fps;
		currentFrame_ = hitStopTime_;
	}
	/// <summary>
	/// ヒットストップのデルタタイムの係数を設定
	/// </summary>
	/// <param name="multiply"></param>
	void SetTimeMultiply(const float& multiply) { 
		LWP::Information::SetDeltaTimeMultiply(multiply); 
	}
#pragma endregion

private:
	// フレームパーセカンド
	float fps = 60.0f;

	// ヒットストップする時間
	float hitStopTime_;
	// 経過時間
	float currentFrame_;

	// 終了しているかを取得
	bool isFinish_;
};