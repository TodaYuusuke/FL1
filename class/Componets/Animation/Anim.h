#pragma once
#include <LWP.h>
#include "AnimationStructs.h"

/// <summary>
/// アニメーション単体クラス
/// </summary>
class Anim
{
public: // コンストラクタ等
	
	// デフォルトコンストラクタ削除
	Anim() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="anim">アニメーション</param>
	/// <param name="data">アニメーションデータ</param>
	Anim(LWP::Resource::Animation* anim, const LWP::Animation::AnimData& data);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Anim() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Start();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

public: // アクセッサ等

	/// <summary>
	/// 終了フラグゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsEnd()  const { return isEnd_; }

	/// <summary>
	/// 再生トラックの指定
	/// </summary>
	/// <param name="type">種類</param>
	Anim& SetTrackType(const int type);

	/// <summary>
	/// イベントの追加
	/// </summary>
	/// <param name="eventID">イベント名 (重複可、適当でいい)</param>
	/// <param name="beginFrame">イベントの開始フレーム</param>
	/// <param name="func">実行される関数ポインタ</param>
	Anim& AddEvent(const std::string eventID, const int beginFrame, std::function<void()> func);

private: // メンバ変数

	// アニメーション本体
	LWP::Resource::Animation* anim_ = nullptr;
	
	// アニメーションデータ
	LWP::Animation::AnimData data_{};

	// アニメーション終了フラグ
	bool isEnd_ = false;
};

