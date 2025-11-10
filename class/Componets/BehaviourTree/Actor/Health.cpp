#include "Health.h"
#include <algorithm>

Health::Health(float maxHp) {
	maxHp_ = maxHp;
	hp_ = maxHp_;
}

Health::~Health() {

}

bool Health::GetIsDead() {
	if (hp_ <= minHp_) return true;
	return false;
}
