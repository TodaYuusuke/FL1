#pragma once
#include <Adapter.h>
#include <LWP.h>

/// <summary>
/// 弾エフェクトの基底クラス
/// </summary>
class IBulletEffect
{
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IBulletEffect() = default;

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IBulletEffect() = default;

public: // 純粋仮想関数

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update() = 0;

public: // アクセッサ等

	/// <summary>
	/// 終了状態ゲッター
	/// </summary>
	/// <returns>終了状態</returns>
	bool GetIsEnd() { return isEnd_; }

	/// <summary>
	/// 終了関数
	/// </summary>
	void Finish() { isEnd_ = true; }

protected: // 継承メンバ変数

	// 終了フラグ
	bool isEnd_ = false;

};
