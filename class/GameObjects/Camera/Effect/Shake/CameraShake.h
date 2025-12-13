#pragma once
#include "../CameraEffect.h"

/// <summary>
/// カメラを揺らす
/// </summary>
class CameraShake : CameraEffect {
public:
	// コンストラクタ
	CameraShake();
	// デストラクタ
	~CameraShake() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
};

