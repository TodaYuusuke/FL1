#include "WaitState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

using namespace LWP;
using namespace LWP::Math;

WaitState::WaitState(BlackBoard* pBlackBoard, NodeResult* nodeResult, const float& waitTime) {
	pBlackBoard_ = pBlackBoard;
	nodeResult_ = nodeResult;
	waitTime_ = waitTime;
}

WaitState::~WaitState() {

}

void WaitState::Init() {

}

void WaitState::Update() {
	if (waitTime_ <= 0.0f) {
		*nodeResult_ = (NodeResult::Success);
		isEnableChangeState_ = true;
		return;
	}

	// “GƒAƒhƒŒƒX‚ðŽæ“¾
	Actor* actor = pBlackBoard_->GetValue<Actor*>("Actor");
	quat_ = actor->GetWorldTF()->rotation;
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->SetRotation(quat_);

	waitTime_--;
}

void WaitState::DebugGui() {

}