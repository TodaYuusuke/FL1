#pragma once
#include "../CameraEffect.h"
#include "../../../../Effect/Utility/DeltaTimer.h"

/// <summary>
/// ビネットエフェクト
/// </summary>
class Vignette : public CameraEffect
{
private: // プライベートなサブクラス

	// ブラー状態
	enum VignetteState {
		START,
		END
	};

public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	Vignette() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="blurStrength">最終的なビネット強度</param>
	Vignette(FollowCamera* camera, const float blurStrength);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Vignette() = default;

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

	/// <summary>
	/// 最大ビネット強度設定
	/// </summary>
	/// <param name="kVignetteStrength">ビネット強度</param>
	void SetMaxStrength(const float kVignetteStrength) { kVignetteStrength_ = kVignetteStrength; }

private: // メンバ変数

	// タイマークラス
	LWP::Utility::DeltaTimer timer_{};

	// 現在のビネット強さ
	float nowStrength_ = 0.0f;
	// 現在の補間タイプ
	LWP::Utility::Easing::Type nowType = LWP::Utility::Easing::Type::Liner;

	// 状態
	int state_ = VignetteState::END;

	// 最終的なビネット強さ
	float kVignetteStrength_ = 0.0f;
};

