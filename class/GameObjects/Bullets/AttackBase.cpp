#include "AttackBase.h"

AttackBase::AttackBase(int hitFragBit) {
	stopController_ = HitStopController::GetInstance();
	// 当たり判定をとる対象のマスクを設定
	hitFragBit_ = hitFragBit;
}
