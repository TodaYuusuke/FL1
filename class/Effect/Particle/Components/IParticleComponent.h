#pragma once
#include <Adapter.h>
#include "math/vector/Vector3.h"
#include "math/vector/Vector4.h"
#include "math/Quaternion.h"

/// <summary>
/// パーティクル機能の基底クラス
/// </summary>
/// <typeparam name="T">任意型</typeparam>
template <typename T>
class IParticleComponent {
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IParticleComponent() = default;

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IParticleComponent() = default;


public: // 純粋仮想関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update(const float deltaTime, const float playSpeed) = 0;

};