#include "ReadyToAttackState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

using namespace LWP;
using namespace LWP::Math;

ReadyToAttackState::ReadyToAttackState(BlackBoard* pBlackBoard, NodeResult* nodeResult, const float& waitTime) {
	pBlackBoard_ = pBlackBoard;
	nodeResult_ = nodeResult;
	waitTime_ = waitTime;
}

ReadyToAttackState::~ReadyToAttackState() {

}

void ReadyToAttackState::Init() {

}

void ReadyToAttackState::Update() {
	if (waitTime_ <= 0.0f) {
		*nodeResult_ = (NodeResult::Success);
		isEnableChangeState_ = true;
		return;
	}

	// 敵アドレスを取得
	Actor* actor = pBlackBoard_->GetValue<Actor*>("Actor");
	quat_ = actor->GetWorldTF()->rotation;
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->SetRotation(quat_);

	waitTime_--;
}

void ReadyToAttackState::DebugGui() {

}