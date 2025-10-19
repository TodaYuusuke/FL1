#include "AttackState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

AttackState::AttackState(BlackBoard* pBlackBoard) {
	pBlackBoard_ = pBlackBoard;

	//attackModel_.LoadCube();
	//attackModel_.worldTF.Parent(pBlackBoard->GetValue<Actor*>(EnemyConfig::name)->GetWorldTF());
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

	// 持っている武器で攻撃
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->Attack();

	currentFrame_--;
}

void AttackState::DebugGui() {

}