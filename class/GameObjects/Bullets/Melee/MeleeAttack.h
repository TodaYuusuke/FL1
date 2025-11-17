#pragma once
#include "../BulletBase.h"
#include <Adapter.h>

class MeleeAttack : public BulletBase {
public:
	// コンストラクタ
	MeleeAttack(const LWP::Math::Vector3& colliderSize, LWP::Object::TransformQuat* target, int hitFragBit, float attackPower, float elapsedTime);
	// デストラクタ
	~MeleeAttack() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
	LWP::Object::TransformQuat* target_;
};

