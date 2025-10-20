#include "WaitLeaf.h"
#include "../Actor/Actor.h"
#include "../../../GameObjects/Enemy/State/WaitState.h"

WaitLeaf::WaitLeaf(BlackBoard* black_board, const float wait_time)
	: LeafNodeBase(black_board)
	, waitTime(wait_time)
	, waitCount(wait_time)
{
}

WaitLeaf::~WaitLeaf() {

}

void WaitLeaf::Tick() {
	// 敵アドレスを取得
	Actor* actor = blackBoard->GetValue<Actor*>("Actor");
	actor->ChangeState(new WaitState(blackBoard, &nodeResult, waitTime));
}

void WaitLeaf::Finalize() {
	LeafNodeBase::Finalize();
	waitCount = waitTime;
}
