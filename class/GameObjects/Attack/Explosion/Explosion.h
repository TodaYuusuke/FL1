#pragma once
#include "../AttackBase.h"

class Explosion : public AttackBase {
public:
	// コンストラクタ
	Explosion(const ImpactData& data, const LWP::Math::Vector3& pos, int hitFragBit);
	// デストラクタ
	~Explosion() override = default;

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

private:
	// AABB
	LWP::Object::Collider::Sphere& bodySphere_;

	// 調整情報
	ImpactData data_;
};

