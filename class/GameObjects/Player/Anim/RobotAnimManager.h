#pragma once
#include <LWP.h>

/// <summary>
/// ロボットアニメーションの管理クラス
/// </summary>
class RobotAnimManager
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	RobotAnimManager() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="animModel">アニメーション対象モデル</param>
	/// <param name="velocity">移動アニメーションブレンド用速度変数のポインタ</param>
	RobotAnimManager(LWP::Resource::SkinningModel* animModel, const LWP::Math::Vector3* velocity = nullptr);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RobotAnimManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

private: // メンバ変数

	// アニメーション対象モデル
	LWP::Resource::SkinningModel* animModel_ = nullptr;


};

