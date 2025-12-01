#pragma once
#include "../BulletBase.h"

class Explosion : public BulletBase {
	// コンストラクタ
	Explosion(const BulletData& data, const LWP::Math::Vector3& pos, int hitFragBit);
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

private:// 調整項目
	// 爆発の最大範囲
	LWP::Math::Vector3 maxExplosionSize = { 10.0f ,10.0f ,10.0f };

private:
};

