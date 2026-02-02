#pragma once
#include "../BulletBase.h"
#include <Adapter.h>

class MeleeAttack : public BulletBase {
public:
	// コンストラクタ
	MeleeAttack(const AttackData& data, LWP::Object::TransformQuat* target, IWeapon* weapon, int hitFragBit);
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
	LWP::Object::TransformQuat* targetTF_;
};

