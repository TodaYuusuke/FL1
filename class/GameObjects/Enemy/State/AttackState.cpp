#include "AttackState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

AttackState::AttackState(BlackBoard* pBlackBoard, NodeResult* nodeResult) {
	pBlackBoard_ = pBlackBoard;
	nodeResult_ = nodeResult;
}

AttackState::~AttackState() {

}

void AttackState::Init() {

}

void AttackState::Update() {
	if (currentFrame_ <= 0.0f) {
		*nodeResult_ = (NodeResult::Success);
		isEnableChangeState_ = true;
		return;
	}

	// 持っている武器で攻撃
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->Attack();

	currentFrame_--;
}

void AttackState::DebugGui() {

}