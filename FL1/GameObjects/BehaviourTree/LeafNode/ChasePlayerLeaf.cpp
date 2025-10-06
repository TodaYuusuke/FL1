#include "ChasePlayerLeaf.h"

#include "Math.h"
#include "../Actor/BlackBoard.h"
#include "../Actor/IAgent.h"

using namespace LWP::Math;

ChasePlayerLeaf::ChasePlayerLeaf(BlackBoard* black_board)
	: LeafNodeBase(black_board)
{
}

ChasePlayerLeaf::~ChasePlayerLeaf()
{
}

void ChasePlayerLeaf::Tick()
{
	// プレイヤーの位置を取得
	auto player_pos = blackBoard->GetValue<Vector3>("PlayerPos");
	auto* agent = blackBoard->GetValue<IAgent*>("Agent");

	auto vector = player_pos - agent->GetPos();

	agent->MoveTowards(vector.Normalize(), 3.25f);
}

NodeResult ChasePlayerLeaf::GetNodeResult() const
{
	// 必ず成功を返す
	return NodeResult::Success;
}
