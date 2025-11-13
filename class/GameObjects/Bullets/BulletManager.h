#pragma once
#include <Adapter.h>
#include "BulletBase.h"

/// <summary>
/// 弾管理クラス
/// </summary>
class BulletManager final : public LWP::Utility::ISingleton<BulletManager> {
	friend class LWP::Utility::ISingleton<BulletManager>;// ISingletonをフレンドクラスにしてコンストラクタを呼び出せるように
private:
	// コンストラクタ
	BulletManager();
	// デストラクタ
	~BulletManager();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:// アクセサ
	/// <summary>
	/// 弾の生成
	/// </summary>
	void CreateBullet(BulletBase* bullet);

	/// <summary>
	/// 特定の弾を検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	BulletBase* FindBullet(const std::string& name) {
		auto result = std::find_if(bullets_.begin(), bullets_.end(),
			[&](BulletBase* bullet) { return bullet->GetName() == name; }
		);
		if (result == bullets_.end()) return nullptr;

		return *result;
	}

private:
	// 弾リスト
	std::list<BulletBase*> bullets_;

	int createID_;
};