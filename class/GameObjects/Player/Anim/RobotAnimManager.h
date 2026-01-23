#pragma once
#include "../../../Componets/Animation/AnimationManager.h"

/// <summary>
/// ロボットアニメーションの管理クラス
/// </summary>
class RobotAnimManager : public AnimationManager
{
public: // サブクラス

	// 再生する箇所の種類
	enum PlayType {
		Other,					// 腕以外
		LeftArm			= 2,	// 左腕
		RightArm		= 3,	// 右腕
		LeftShoulder	= 4,	// 左肩
		RightShoulder	= 5,	// 右肩
	};

public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	RobotAnimManager() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="filePath">モデルまでのファイルパス</param>
	/// <param name="model">アニメーション対象モデル</param>
	/// <param name="moveVec">移動ベクトルのポインタ</param>
	RobotAnimManager(const std::string& filePath, LWP::Resource::SkinningModel* model, const LWP::Math::Vector3* moveVec);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RobotAnimManager();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// 再生キューに再生命令を追加する関数
	/// </summary>
	/// <param name="animName">アニメーション名</param>
	/// <param name="trackType">再生する箇所</param>
	/// <param name="transitionTime">遷移秒数</param>
	/// <param name="isLoop">ループを行うか</param>
	Anim& PlayQue(const std::string& animName, const int trackType, const float transitionTime = 0.0f, const bool isLoop = false);

	/// <summary>
	/// キューを無視して直接再生を行う関数
	/// </summary>
	/// <param name="animName">アニメーション名</param>
	/// <param name="transitionTime">遷移秒数</param>
	/// <param name="isLoop">ループを行うか</param>
	/// <param name="trackType">(任意)再生するトラック</param>
	Anim& PlayDirect(const std::string& animName, const int trackType, const float transitionTime = 0.0f, const bool isLoop = false);

private: // プライベートなメンバ関数

	/// <summary>
	/// アニメーションキュー更新関数
	/// </summary>
	/// <param name="animQue">キュー</param>
	void AnimQueUpdate(std::list<Anim*>& animQue);

	/// <summary>
	/// 移動時のブレンド更新
	/// </summary>
	void MoveBlendUpdate();

	/// <summary>
	/// 渡された二次元ベクトルを元にアニメーション時間を求める関数
	/// </summary>
	/// <param name="v">二次元ベクトル</param>
	/// <returns>アニメーション時間用T (0.0f ~ 1.0f) </returns>
	float CalcMoveT(const LWP::Math::Vector2& v);

private: // メンバ変数

	// 左腕
	std::list<Anim*> leftArmAnimQue_{};
	// 右腕
	std::list<Anim*> rightArmAnimQue_{};

	// 左肩
	std::list<Anim*> leftShoulderAnimQue_{};
	// 右肩
	std::list<Anim*> rightShoulderAnimQue_{};

	// ブレンド時に使用する移動ベクトルのポインタ
	const LWP::Math::Vector3* moveVelocity = nullptr;
};

