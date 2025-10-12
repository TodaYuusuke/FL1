#include "WaitState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

using namespace LWP;
using namespace LWP::Math;

WaitState::WaitState(BlackBoard* pBlackBoard, const float& waitTime) {
	pBlackBoard_ = pBlackBoard;
	waitTime_ = waitTime;
}

WaitState::~WaitState() {

}

void WaitState::Init() {

}

void WaitState::Update() {
	if (waitTime_ <= 0.0f) {
		isEnableChangeState_ = true;
		return;
	}

	waitTime_--;
}

void WaitState::DebugGui() {

}