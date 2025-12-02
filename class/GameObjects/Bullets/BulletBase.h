#pragma once
#include "../../Componets/Math.h"
#include "../../Componets/HitStopController.h"
#include "BulletConfig.h"
#include "AttackBase.h"
#include "Strategy/Impact/IImpact.h"
#include <Adapter.h>

/// <summary>
/// 弾の基底クラス
/// </summary>
class BulletBase : public AttackBase {
public:
	// コンストラクタ
	BulletBase(const AttackData& data, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel = { 0,0,0 });
	// デストラクタ
	~BulletBase() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 衝突応答
	/// </summary>
	void OnCollision(LWP::Object::Collision* hitTarget) override;

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

protected:
	// 着弾時の処理
	std::unique_ptr<IImpact> impact_;

	// AABB
	LWP::Object::Collider::AABB& bodyAABB_;

	// 調整情報
	AttackData data_;
};