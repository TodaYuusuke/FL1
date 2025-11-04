#pragma once
#include "../../Componets/Math.h"
#include "../../Componets/HitStopController.h"
#include <Adapter.h>

/// <summary>
/// 弾の基底クラス
/// </summary>
class BulletBase {
public:
	// コンストラクタ
	BulletBase();
	// デストラクタ
	virtual ~BulletBase() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

protected:
	/// <summary>
	/// 衝突応答
	/// </summary>
	virtual void OnCollision(LWP::Object::Collision* hitTarget);

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 生存しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAlive() { return isAlive_; }
#pragma endregion

#pragma region Setter

#pragma endregion

protected:
	// ヒットストップ
	HitStopController* stopController_;

	// モデル
	LWP::Resource::RigidModel body_;

	// 体の当たり判定
	LWP::Object::Collision bodyCollision_;
	LWP::Object::Collider::AABB& bodyAABB_;

	// 速度
	LWP::Math::Vector3 vel_ = { 0.0f,0.0f,1.0f };

	// 生存しているか
	bool isAlive_ = true;
};