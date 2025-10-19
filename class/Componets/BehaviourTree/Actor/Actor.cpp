#include "Actor.h"
#include "../../../GameObjects/Enemy/State/StateBase.h"

void Actor::Init() {}

void Actor::Update(){}

void Actor::DrawGui() {}

void Actor::Attack() {}

void Actor::Die() {}

void Actor::ChangeState(StateBase* nextState) {
	// 状態が生成されているときだけ解放
	if (state_) delete state_;
	state_ = nextState;
}