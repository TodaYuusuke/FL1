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
	/// 新規平面エミッタの生成
	/// </summary>
	/// <param name="effectName">生成するエフェクト名称</param>
	/// <param name="pos">エミッタの初期座標</param>
	/// <param name="parent">(任意)親となるエミッタ座標</param>
	/// <param name="texPath">(任意)テクスチャ変更する際はパスを入力</param>
	void CreateNewEmitter(std::string effectName, const LWP::Math::Vector3& pos, LWP::Object::TransformQuat* parent = nullptr);

	/// <summary>
	/// (呼び出し禁止)(エディタ用) 新規保存用データ生成
	/// </summary>
	/// <param name="dataName">データ名</param>
	/// <returns>新規データのポインタ</returns>
	LWP::Effect::EffectSaveData* CreateNewData(const std::string& dataName);

	/// <summary>
	/// (呼び出し禁止)(エディタ用) 新規保存用データ生成
	/// </summary>
	/// <returns>データ配列内に存在する全てのパーティクルの名称を取得する</returns>
	std::vector<std::string> GetDatas();

	/// <summary>
	/// (呼び出し禁止)(エディタ用) 全てのデータの保存
	/// </summary>
	void SaveAllData();

	/// <summary>
	/// (呼び出し禁止)(エディタ用) エフェクトデータの読み込み
	/// </summary>
	void LoadEffectData();

private: // メンバ変数

	// エミッタリスト
	std::list<Emitter*> emitters_{};

	// 保存データのディレクトリ
	std::string filePath_ = "Particles/Particles.json";
	// 保存データ読み込み用
	LWP::Utility::JsonIO jsonIO_{};
	// エフェクトの保存データリスト
	std::map<std::string, LWP::Effect::EffectSaveData*> effectDatas_{};

	// 粒子全体の再生速度
	float playSpeed_ = 1.0f;

};

