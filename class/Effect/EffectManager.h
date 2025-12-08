#pragma once
#include <Adapter.h>
#include "Emitter/Emitter.h"

/// <summary>
/// エフェクト管理クラス
/// </summary>
class EffectManager
{
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffectManager();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグGUI関数
	/// </summary>
	void DebugGUI();

private: // メンバ変数

	// エミッタリスト
	std::list<Emitter*> emitters_{};

	// 粒子全体の再生速度
	float playSpeed_ = 1.0f;

};

