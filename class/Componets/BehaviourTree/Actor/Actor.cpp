#include "Actor.h"
#include "../../../GameObjects/Enemy/State/StateBase.h"

void Actor::Init() {}

void Actor::Update(){}

void Actor::DrawGui() {}

void Actor::Attack() {}

void Actor::Die() {}

void Actor::ChangeState(StateBase* nextState) {
	// ó‘Ô‚ª¶¬‚³‚ê‚Ä‚¢‚é‚Æ‚«‚¾‚¯‰ğ•ú
	if (state_) delete state_;
	state_ = nextState;
}