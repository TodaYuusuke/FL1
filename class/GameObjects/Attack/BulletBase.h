#pragma once
#include "../../Componets/Math.h"
#include "../../Componets/HitStopController.h"
#include "AttackConfig.h"
#include "AttackBase.h"
#include "Strategy/Impact/IImpact.h"
#include "Strategy/Movement/IMovement.h"
#include <Adapter.h>

class Actor;
/// <summary>
/// 弾の基底クラス
/// </summary>
class BulletBase : public AttackBase {
public:
	// コンストラクタ
	BulletBase(const AttackData& data, Actor* target, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel = { 0,0,0 });
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

	/// <summary>
	/// 移動方式を変更
	/// </summary>
	/// <param name="movement"></param>
	void ChangeMovement(std::unique_ptr<IMovement> movement) { movement_ = std::move(movement); }

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 弾の調整情報を取得
	/// </summary>
	/// <returns></returns>
	AttackData GetAttackData() { return data_; }
#pragma endregion

#pragma region Setter

#pragma endregion

protected:
	// 着弾時の処理
	std::unique_ptr<IImpact> impact_;
	// 移動方式
	std::unique_ptr<IMovement> movement_;

	// AABB
	LWP::Object::Collider::AABB& bodyAABB_;

	// 調整情報
	AttackData data_;
};