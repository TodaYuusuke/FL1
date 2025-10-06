#include "EscapeFromPlayerLeaf.h"

#include "Math.h"
#include "../Actor/BlackBoard.h"
#include "../Actor/IAgent.h"

using namespace LWP::Math;

EscapeFromPlayerLeaf::EscapeFromPlayerLeaf(BlackBoard* black_board)
	: LeafNodeBase(black_board)
{
}

EscapeFromPlayerLeaf::~EscapeFromPlayerLeaf()
{
}

void EscapeFromPlayerLeaf::Tick()
{
	// プレイヤーの位置を取得
	auto player_pos = blackBoard->GetValue<Vector3>("PlayerPos");
	auto* agent = blackBoard->GetValue<IAgent*>("Agent");

	auto vector = agent->GetPos() - player_pos;

	agent->MoveTowards(vector.Normalize(), 3.f);
}

NodeResult EscapeFromPlayerLeaf::GetNodeResult() const
{
	// 必ず成功を返す
	return NodeResult::Success;
}
