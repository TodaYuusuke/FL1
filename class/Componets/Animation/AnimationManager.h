#pragma once
#include <LWP.h>
#include "Anim.h"

/// <summary>
/// アニメーション管理クラス(基底クラスとして使用可)
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
	Anim& PlayQue(const std::string& animName, const float transitionTime = 0.0f, const bool isLoop = false);
	
	/// <summary>
	/// <para> キューを無視して直接再生を行う関数 </para>
	/// <para> !! 必ずStart関数も同時に呼び出すように !! </para>
	/// <para> 詳細は 参照</para>
	/// </summary>
	/// <param name="animName">アニメーション名</param>
	/// <param name="transitionTime">遷移秒数</param>
	/// <param name="isLoop">ループを行うか</param>
	Anim& PlayDirect(const std::string& animName, const float transitionTime = 0.0f, const bool isLoop = false);

protected: // メンバ変数

	// アニメーション
	LWP::Resource::Animation* animation_ = nullptr;

	// アニメーションキュー
	std::list<Anim*> animQue_{};
};

