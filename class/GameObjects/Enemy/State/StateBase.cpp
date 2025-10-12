#include "StateBase.h"

void StateBase::ChangeState(StateBase* currentState, StateBase* nextState) {
	// 状態が生成されているときだけ解放
	if(currentState) delete currentState;
	currentState = nextState;
}
