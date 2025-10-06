#include "CheckFarPlayer.h"
#include "../Actor/BlackBoard.h"
#include "../Actor/IAgent.h"
#include "Math.h"

using namespace LWP::Math;

CheckFarPlayer::CheckFarPlayer(BlackBoard* black_board, INode* true_node, INode* false_node, const float max_distance)
	: BranchNodeBase(black_board, true_node, false_node),
	maxDistance(max_distance) {
}

CheckFarPlayer::~CheckFarPlayer()
{
}

const bool CheckFarPlayer::IsCondition()
{
	// プレイヤーの位置を取得
	Vector3 player_pos = blackBoard->GetValue<Vector3>("PlayerPos");
	IAgent* agent = blackBoard->GetValue<IAgent*>("Agent");

	Vector3 vector = player_pos - agent->GetPos();
	float result = std::sqrt((vector.x * vector.x) + (vector.y * vector.y));

	return result >= maxDistance;
}