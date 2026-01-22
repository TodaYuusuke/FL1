#pragma once
#include <LWP.h>
#include "Anim.h"

/// <summary>
/// アニメーション管理クラス
/// </summary>
class AnimationManager
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	AnimationManager() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="anim">対象アニメーション</param>
	AnimationManager(LWP::Resource::Animation* anim);

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~AnimationManager();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update();

	/// <summary>
	/// デバッグGUI関数
	/// </summary>
	virtual void DebugGUI(const std::string& id);

	/// <summary>
	/// 再生キューに再生命令を追加する関数
	/// </summary>
	/// <param name="animName">アニメーション名</param>
	/// <param name="transitionTime">遷移秒数</param>
	/// <param name="isLoop">ループを行うか</param>
	/// <param name="trackType">(任意)再生するトラック</param>
	virtual Anim& PlayQue(const std::string& animName, const float transitionTime = 0.0f, const bool isLoop = false, const int trackType = 0);
	
	/// <summary>
	/// キューを無視して直接再生を行う関数
	/// </summary>
	/// <param name="animName">アニメーション名</param>
	/// <param name="transitionTime">遷移秒数</param>
	/// <param name="isLoop">ループを行うか</param>
	/// <param name="trackType">(任意)再生するトラック</param>
	virtual Anim& PlayDirect(const std::string& animName, const float transitionTime = 0.0f, const bool isLoop = false, const int trackType = 0);

protected: // メンバ変数

	// アニメーション
	LWP::Resource::Animation* animation_ = nullptr;

	// アニメーションキュー
	std::list<Anim*> animQue_{};

#pragma region デバッグ用変数

	// 停止フラグ
	bool isStop_ = false;

	// 選択中ID
	int selectedID_ = 0;

	// 選択中のアニメーション
	Anim* debugingAnim_ = nullptr;

#pragma endregion

};

