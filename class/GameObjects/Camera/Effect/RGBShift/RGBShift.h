#pragma once
#include "../CameraEffect.h"
#include "../../../../Effect/Utility/DeltaTimer.h"

/// <summary>
/// RGBずらし
/// </summary>
class RGBShift : public CameraEffect
{
private: // プライベートなサブクラス

	// ずらし状態
	enum RGBShiftState {
		START,
		END
	};

public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	RGBShift() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="shiftRGB">RGBずらし値</param>
	RGBShift(FollowCamera* camera, const LWP::Math::Vector3& shiftRGB);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RGBShift() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 開始関数
	/// </summary>
	/// <param name="startTime">開始秒数</param>
	/// <param name="startType">開始補間タイプ</param>
	void Start(const float startTime, const LWP::Utility::Easing::Type& startType = LWP::Utility::Easing::Type::Liner);
	/// <summary>
	/// 終了関数
	/// </summary>
	/// <param name="endTime">終了秒数</param>
	/// <param name="endType">終了補間タイプ</param>
	void Finish(const float endTime, const LWP::Utility::Easing::Type& endType = LWP::Utility::Easing::Type::Liner);

public: // アクセッサ等

	/// <summary>
	/// 最大ずらし強度設定
	/// </summary>
	/// <param name="kVignetteStrength">ずらし強度</param>
	void SetMaxStrength(const LWP::Math::Vector3& kRGBShiftStrength) { kRGBShiftStrength_ = kRGBShiftStrength; }

	/// <summary>
	/// 現在の状態取得
	/// </summary>
	/// <returns></returns>
	int GetIsState() { return state_; }

	/// <summary>
	/// 演出中かどうかの取得
	/// </summary>
	/// <returns>演出中か</returns>
	bool GetIsStaging() { return timer_.GetIsActive(); }

private: // メンバ変数

	// タイマークラス
	LWP::Utility::DeltaTimer timer_{};

	// 現在のずらし強度
	LWP::Math::Vector3 nowStrength_{};
	// 現在の補間タイプ
	LWP::Utility::Easing::Type nowType = LWP::Utility::Easing::Type::Liner;

	// 状態
	int state_ = RGBShiftState::END;

	// 最終的なRGBシフトの強さ
	LWP::Math::Vector3 kRGBShiftStrength_{}; // 最大
};

