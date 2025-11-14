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

void Health::Damage(float value, const std::string& name) {
	isHit_ = true;

	// 多重被弾回避
	if (!damageAttackerName_.empty()) {
		auto result = std::find(damageAttackerName_.begin(), damageAttackerName_.end(), name);
		if (result != damageAttackerName_.end()) {
			return;
		}
	}

	// 攻撃者の名前
	damageAttackerName_.push_back(name);
	hp_ = std::clamp<float>(hp_ - value, minHp_, maxHp_);
}

bool Health::GetIsDead() {
	if (hp_ <= minHp_) return true;
	return false;
}
