#pragma once

#include "LeafNodeBase.h"

/// <summary>
/// プレイヤーを追いかける葉ノード
/// </summary>
class ChasePlayerLeaf : public LeafNodeBase {
public:
	explicit ChasePlayerLeaf(BlackBoard* black_board, const float& speed);

	~ChasePlayerLeaf();

	void Tick() override;

	NodeResult GetNodeResult() const override;

private:
	float speed = 0.0f;
};