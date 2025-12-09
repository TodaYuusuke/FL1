#pragma once
#include <Adapter.h>
#include "Emitter/Emitter.h"

/// <summary>
/// エフェクト管理クラス
/// </summary>
class EffectManager final : public LWP::Utility::ISingleton<EffectManager>{
	friend class LWP::Utility::ISingleton<EffectManager>;
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
	/// 新規エミッタの生成
	/// </summary>
	/// <param name="ParticleName">生成するパーティクル名称</param>
	/// <param name="pos">エミッタの初期座標</param>
	/// <param name="parent">(任意)親となるエミッタ座標</param>
	void CreateNewEmitter(std::string ParticleName, const LWP::Math::Vector3& pos, LWP::Object::TransformQuat* parent = nullptr);

	/// <summary>
	/// (呼び出し禁止) 新規エミッタの受け取り
	/// </summary>
	/// <param name="newEmitter">新規エミッタ</param>
	void ReciveNewEmitter(Emitter* newEmitter);

private: // メンバ変数

	// エミッタリスト
	std::list<Emitter*> emitters_{};

	// エフェクトの保存データリスト
	std::map<std::string, LWP::Effect::EffectSaveData> effectDatas_{};

	// 粒子全体の再生速度
	float playSpeed_ = 1.0f;

};

