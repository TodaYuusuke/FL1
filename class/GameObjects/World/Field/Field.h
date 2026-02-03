#pragma once
#include <LWP.h>
#include <Adapter.h>
#include "../../PenetrationResolver/PenetrationResolver.h"
#include "Prop.h"

/// <summary>
/// フィールドクラス
/// </summary>
class Field {
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Field();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Field();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// GUI表示関数
	/// </summary>
	void DebugGUI();

	/// <summary>
	/// 最終処理
	/// </summary>
	void EndFrame();

private: // メンバ関数

	/// <summary>
	/// 新規配置物作成
	/// </summary>
	/// <param name="name">配置物名称</param>
	/// <param name="filePath">モデルまでのファイルパス</param>
	void CreateNewProp(const std::string& name, const std::string filePath);

	/// <summary>
	/// 保存関数
	/// </summary>
	void SaveAllData();

	/// <summary>
	/// 読み込み関数
	/// </summary>
	void LoadProps();

private:
	
	// 貫通防止クラス
	PenetrationResolver* resolver_ = nullptr;

	//　配置物配列
	std::list<Prop*> props_{};

	// 保存データのディレクトリ
	std::string filePath_ = "Level/PropData.json";
	// 保存データ読み込み用
	LWP::Utility::JsonIO jsonIO_{};

#pragma region デバッグ用変数

	// 追加する配置物名称
	std::string addPropName_ = "Prop";
	// 追加する配置物のモデルパス
	std::string addModelpath_ = "";

	// 選択中ID
	int selectedID_ = 0;

	// 選択中の接地物
	Prop* selectingProp_ = nullptr;

#pragma endregion
};