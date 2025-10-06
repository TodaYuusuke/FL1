#pragma once

#include "LeafNodeBase.h"

/// <summary>
/// 指定した時間待機する葉ノード
/// </summary>
class WaitLeaf : public LeafNodeBase {
public:
	explicit WaitLeaf(BlackBoard* black_board, const float wait_time);
	~WaitLeaf();

	void Tick() override;
	void Finalize() override;

private:
	float waitTime = 0.f;
	float waitCount = 0.f;
};