#pragma once
#include <Adapter.h>
#include "../FollowCamera/FollowCamera.h"
#include "CameraEffect.h"

/// <summary>
/// カメラの演出を実行
/// </summary>
class CameraEffectHandler {
	// コンストラクタ
	CameraEffectHandler();
	// デストラクタ
	~CameraEffectHandler() = default;

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
	/// 演出開始
	/// </summary>
	/// <param name="effectType"></param>
	void StartEffect(int effectType);

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
	// 演出処理
	//std::map<int, CameraEffect> cameraEffects_;
};

