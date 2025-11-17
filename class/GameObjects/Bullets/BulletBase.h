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
	BulletBase(float attackPower, float speed, float elapsedTime);
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
	/// 名前を取得
	/// </summary>
	/// <returns></returns>
	std::string GetName() { return bodyCollision_.name; }
	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	/// <returns></returns>
	float GetAttackPower() { return attackPower_; }
	/// <summary>
	/// 生存しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAlive() { return isAlive_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="name"></param>
	void SetName(const std::string& name) { bodyCollision_.name = name; }
	/// <summary>
	/// 攻撃力を設定
	/// </summary>
	/// <returns></returns>
	void SetAttackPower(float value) { attackPower_ = value; }
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

	// 攻撃力
	float attackPower_;
	// 移動速度
	float moveSpeed_;

	float currentFrame_ = 60.0f;

	// 生存しているか
	bool isAlive_ = true;
};