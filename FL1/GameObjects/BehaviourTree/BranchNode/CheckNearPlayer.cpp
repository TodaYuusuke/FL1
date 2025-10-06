#include "CheckNearPlayer.h"

#include "Math.h"
#include "../Actor/BlackBoard.h"
#include "../Actor/IAgent.h"

using namespace LWP::Math;

CheckNearPlayer::CheckNearPlayer(BlackBoard* black_board, INode* true_node, INode* false_node, const float max_distance)
	: BranchNodeBase(black_board, true_node, false_node),
	maxDistance(max_distance) {
}

CheckNearPlayer::~CheckNearPlayer()
{
}

const bool CheckNearPlayer::IsCondition()
{
	// プレイヤーの位置を取得
	Vector3 player_pos = blackBoard->GetValue<Vector3>("PlayerPos");
	IAgent* agent = blackBoard->GetValue<IAgent*>("Agent");

	Vector3 vector = player_pos - agent->GetPos();
	float result = std::sqrt((vector.x * vector.x) + (vector.y * vector.y));

	return result < maxDistance;
}