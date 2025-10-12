#include "CheckNearPlayer.h"
#include "Math.h"
#include "../Actor/BlackBoard.h"
#include "../Actor/Actor.h"

using namespace LWP::Math;

CheckNearPlayer::CheckNearPlayer(BlackBoard* black_board, INode* true_node, INode* false_node, const float max_distance)
	: BranchNodeBase(black_board, true_node, false_node),
	maxDistance(max_distance) {
}

CheckNearPlayer::~CheckNearPlayer() {
}

const bool CheckNearPlayer::IsCondition() {
	// 自機アドレスを取得
	Actor* player = blackBoard->GetValue<Actor*>("Player");
	// 敵アドレスを取得
	Actor* actor = blackBoard->GetValue<Actor*>("Actor");

	// 方向ベクトル算出
	Vector3 vector = player->GetWorldTF()->GetWorldPosition() - actor->GetWorldTF()->GetWorldPosition();
	// 距離を算出
	float result = std::sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	return result < maxDistance;
}