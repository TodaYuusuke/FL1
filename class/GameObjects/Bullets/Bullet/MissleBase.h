#pragma once
#include "../BulletBase.h"

class MissleBase : public BulletBase {
public:
	// コンストラクタ
	MissleBase(const BulletData& data, const LWP::Math::Vector3& pos, int hitFragBit, const LWP::Math::Vector3& dirVel);
	// デストラクタ
	~MissleBase() = default;

	/// <summary>
	/// 衝突応答
	/// </summary>
	/// <param name="hitTarget"></param>
	void OnCollision(LWP::Object::Collision* hitTarget) override;

private:

};