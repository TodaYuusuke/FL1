#include "Health.h"
#include <algorithm>

Health::Health(float maxHp) {
	maxHp_ = maxHp;
	hp_ = maxHp_;
}

Health::~Health() {

}

void Health::Update() {
	if (!isHit_) {
		damageAttackerName_.clear();
	}

	isHit_ = false;
}

void Health::Damage(float value) {
	hp_ = std::clamp<float>(hp_ - value, minHp_, maxHp_);
	isHit_ = true;
}

bool Health::GetIsDead() {
	if (hp_ <= minHp_) return true;
	return false;
}
