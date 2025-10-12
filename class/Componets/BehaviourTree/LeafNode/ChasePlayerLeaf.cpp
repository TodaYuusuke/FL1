#include "ChasePlayerLeaf.h"
#include "../Actor/BlackBoard.h"
#include "../Actor/Actor.h"
#include "../../../GameObjects/Enemy/State/ChaseState.h"

using namespace LWP::Math;

ChasePlayerLeaf::ChasePlayerLeaf(BlackBoard* black_board, const float& speed)
	: LeafNodeBase(black_board)
{
	this->speed = speed;
}

ChasePlayerLeaf::~ChasePlayerLeaf() {

}

void ChasePlayerLeaf::Tick() {
	// 敵アドレスを取得
	Actor* actor = blackBoard->GetValue<Actor*>("Actor");
	actor->ChangeState(new ChaseState(blackBoard, speed));
}

NodeResult ChasePlayerLeaf::GetNodeResult() const {
	// 必ず成功を返す
	return NodeResult::Success;
}
