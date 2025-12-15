#pragma once
#include <Adapter.h>
#include "../CameraConfig.h"
#include "../FollowCamera/FollowCamera.h"
#include "Shake/CameraShake.h"
#include "Zoom/CameraZoom.h"
#include "CameraEffect.h"

/// <summary>
/// カメラの演出を実行
/// </summary>
class CameraEffectHandler final : public LWP::Utility::ISingleton<CameraEffectHandler> {
	friend class LWP::Utility::ISingleton<CameraEffectHandler>;
private:
	// コンストラクタ
	CameraEffectHandler();
	// デストラクタ
	~CameraEffectHandler() = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:// アクセサ
	/// <summary>
	/// 揺れ演出開始
	/// </summary>
	/// <param name="effectType"></param>
	void StartShake(LWP::Math::Vector3 range, float endTime);
	/// <summary>
	/// ズーム演出開始
	/// </summary>
	/// <param name="zoomValue"></param>
	/// <param name="endTime"></param>
	void StartZoom(float zoomValue, float endTime);

#pragma region Getter

#pragma endregion

#pragma region Setter
	/// <summary>
	/// カメラ演出をさせるカメラを設定
	/// </summary>
	/// <param name="target"></param>
	void SetEffectTarget(FollowCamera* target) { effectTarget_ = target; }
#pragma endregion

private:
	// 演出対象
	FollowCamera* effectTarget_;

	std::unique_ptr<CameraShake> shake_;

	// 演出処理
	std::map<CameraEffectType, std::unique_ptr<CameraEffect>> cameraEffects_;
};

