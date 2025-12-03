#pragma once
#include "IMovement.h"

class StraightMove : public IMovement {
public:
	// コンストラクタ
	StraightMove();
	// デストラクタ
	~StraightMove() override = default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="b"></param>
	void Update(BulletBase* b) override;
};