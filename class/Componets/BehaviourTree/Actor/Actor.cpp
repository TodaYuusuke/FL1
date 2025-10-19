#include "Actor.h"
#include "../../../GameObjects/Enemy/State/StateBase.h"
#include "../../../GameObjects/Weapon/IWeapon.h"

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

void Actor::ChangeWeapon(IWeapon* nextWeapon) {
	// 武器がすでに生成されているときだけ解放
	if (weapon_) delete weapon_;
	weapon_ = nextWeapon;
}
