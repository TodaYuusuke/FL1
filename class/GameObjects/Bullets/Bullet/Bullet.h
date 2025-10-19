#pragma once
#include "../BulletBase.h"

class Bullet : public BulletBase {
public:
	// コンストラクタ
	Bullet(const LWP::Math::Vector3& pos, const LWP::Math::Vector3& dirVel);
	// デストラクタ
	~Bullet() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
	float currentFrame_ = 300.0f;
};