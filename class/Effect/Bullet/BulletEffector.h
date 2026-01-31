#pragma once
#include "BulletEffectIncluder.h"

/// <summary>
/// 全弾の弾道エフェクター
/// </summary>
class BulletEffector final : public LWP::Utility::ISingleton<BulletEffector> {
	friend class LWP::Utility::ISingleton<BulletEffector>;
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BulletEffector() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BulletEffector();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="mainCamera">使用中カメラ</param>
	void Init(LWP::Object::Camera* mainCamera);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 弾道エフェクト生成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="transform">座標</param>
	/// <param name="vec">弾のベクトル</param>
	/// <returns>生成した弾道エフェクト</returns>
	IBulletEffect* CreateBulletLineEffect(const LWP::Math::Vector3& scale, LWP::Object::TransformQuat* transform, const LWP::Math::Vector3* vec);

private: // メンバ変数

	// 使用中カメラ
	LWP::Object::Camera* mainCamera_ = nullptr;

	// 射撃エフェクト配列
	std::list<IBulletEffect*> effects_;

};
