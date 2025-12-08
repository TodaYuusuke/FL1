#pragma once
#include <Adapter.h>
#include "EffectEditor.h"

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

public: // アクセッサ等

	/// <summary>
	/// (デバッグ用) 新規エミッタの生成
	/// </summary>
	/// <param name="newEmitter">新規エミッタ</param>
	void SendNewEmitter(Emitter* newEmitter);

private: // メンバ変数

	// エミッタリスト
	std::list<Emitter*> emitters_{};

	// 粒子全体の再生速度
	float playSpeed_ = 1.0f;

	// エフェクトエディタ
	EffectEditor* effectEditor_ = nullptr;

};

