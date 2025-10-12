#include "AttackState.h"

AttackState::AttackState(BlackBoard* pBlackBoard) {
	pBlackBoard_ = pBlackBoard;

	attackModel_.LoadCube();
}

AttackState::~AttackState() {

}

void AttackState::Init() {

}

void AttackState::Update() {
	if (currentFrame_ <= 0.0f) {
		isEnableChangeState_ = true;
		return;
	}

	currentFrame_--;
}

void AttackState::DebugGui() {

}