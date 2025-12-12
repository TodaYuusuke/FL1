#pragma once
#include <Adapter.h>

class CameraEffect {
public:
	// コンストラクタ
	CameraEffect() = default;
	// デストラクタ
	virtual ~CameraEffect() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
};

