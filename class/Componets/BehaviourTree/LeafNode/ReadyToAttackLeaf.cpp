#include "ReadyToAttackLeaf.h"
#include "../Actor/Actor.h"
#include "../../../GameObjects/Enemy/State/ReadyToAttackState.h"

ReadyToAttackLeaf::ReadyToAttackLeaf(BlackBoard* blackBoard, const float& waitTime, const float& turnTime, const float& speed)
	: LeafNodeBase(blackBoard)
	, waitTime_(waitTime)
	, waitCount_(waitTime)
	, turnTime_(turnTime)
	, moveSpeed_(speed)
{
}

ReadyToAttackLeaf::~ReadyToAttackLeaf() {

}

void ReadyToAttackLeaf::Tick() {
	// 敵アドレスを取得
	Actor* actor = blackBoard->GetValue<Actor*>("Actor");
	actor->ChangeState(new ReadyToAttackState(blackBoard, &nodeResult, waitTime_,turnTime_, moveSpeed_));
}

void ReadyToAttackLeaf::Finalize() {
	LeafNodeBase::Finalize();
	waitCount_ = waitTime_;
}
