#include "CircleAttackLeaf.h"

#include "../Actor/BlackBoard.h"
#include "../Actor/IAgent.h"

CircleAttackLeaf::CircleAttackLeaf(BlackBoard* black_board)
	: LeafNodeBase(black_board)
{
}

CircleAttackLeaf::~CircleAttackLeaf()
{
}

void CircleAttackLeaf::Tick()
{
	// 攻撃
	auto* agent = blackBoard->GetValue<IAgent*>("Agent");
	agent->Attack();
}

NodeResult CircleAttackLeaf::GetNodeResult() const
{
	// 必ず成功を返す
	return NodeResult::Success;
}
