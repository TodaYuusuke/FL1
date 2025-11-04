#include "ReadyToAttackLeaf.h"
#include "../Actor/Actor.h"
#include "../../../GameObjects/Enemy/State/ReadyToAttackState.h"

ReadyToAttackLeaf::ReadyToAttackLeaf(BlackBoard* black_board, const float wait_time)
	: LeafNodeBase(black_board)
	, waitTime(wait_time)
	, waitCount(wait_time)
{
}

ReadyToAttackLeaf::~ReadyToAttackLeaf() {

}

void ReadyToAttackLeaf::Tick() {
	// 敵アドレスを取得
	Actor* actor = blackBoard->GetValue<Actor*>("Actor");
	actor->ChangeState(new ReadyToAttackState(blackBoard, &nodeResult, waitTime));
}

void ReadyToAttackLeaf::Finalize() {
	LeafNodeBase::Finalize();
	waitCount = waitTime;
}
