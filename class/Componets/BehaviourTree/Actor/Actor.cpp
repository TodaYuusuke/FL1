#include "Actor.h"
#include "../../../GameObjects/Enemy/State/StateBase.h"
#include "../../../GameObjects/Weapon/IWeapon.h"

void Actor::Init() {}

void Actor::Update(){
	// 状態
	if(state_) state_->Update();

	// ビヘイビアツリー更新
	if (!state_ || state_->GetIsEnableChangeState()) {
		bt_->Tick();
	}

	if (weapon_) weapon_->Update();
}

void Actor::DrawGui() {}

void Actor::Attack() {
	if(weapon_) weapon_->Attack();
}

void Actor::Die() {}

void Actor::ChangeState(StateBase* nextState) {
	if (state_ && typeid(*nextState) == typeid(*state_)) { return; }
	// 状態が生成されているときだけ解放
	if (state_) delete state_;
	state_ = nextState;
}

void Actor::ChangeWeapon(IWeapon* nextWeapon) {
	// 武器がすでに生成されているときだけ解放
	if (weapon_) delete weapon_;
	weapon_ = nextWeapon;
}
