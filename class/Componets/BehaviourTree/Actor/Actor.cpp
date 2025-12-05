#include "Actor.h"
#include "../../../GameObjects/World/World.h"
#include "../../../GameObjects/Enemy/State/StateBase.h"
#include "../../../GameObjects/Weapon/IWeapon.h"
#include "../../../GameObjects/Collision/CollisionMask.h"

using namespace LWP;
using namespace LWP::Math;

Actor::Actor()
	: bodyAABB_(bodyCollision_.SetBroadShape<LWP::Object::Collider::AABB>())
{

}

void Actor::Init() {}

void Actor::Update() {
	// 前回の座標
	SetPreTranslation(model_.GetJointWorldPosition("LockOnAnchor"));

	// 体力がなければ死亡
	if (hp_->GetIsDead()) { isAlive_ = false; }

	// HP
	hp_->Update();

	// 状態
	if (state_) state_->Update();

	// ビヘイビアツリー更新
	if (!state_ || state_->GetIsEnableChangeState()) {
		bt_->Tick();
	}

	// 速度と角度代入
	if (state_) {
		velocity_ = state_->GetVel();
		quat_ = state_->GetRot();
	}

	model_.worldTF.translation += (velocity_ + weaponVel_) * data_.speedMultiply;

	// 速度を初期化
	velocity_ = { 0.0f, 0.0f, 0.0f };
	weaponVel_ = { 0.0f, 0.0f, 0.0f };
}

void Actor::DrawGui() {}

void Actor::CreateJsonData() {
	// モデル非表示
	model_.isActive = false;
	// 当たり判定をとらない
	bodyCollision_.isActive = false;
}

void Actor::Attack() {
	for (int i = 0; i < weapons_.size(); i++) {
		if (weapons_[i]) {
			// 近接武器なら自機を指定
			if (weapons_[i]->GetWeaponData().name == WeaponConfig::Name::name[(int)WeaponType::kMelee]) {
				// 攻撃
				weapons_[i]->Attack(GameMask::player, GameMask::attack | GameMask::enemy, blackBoard_->GetValue<Actor*>("Player"));
				continue;
			}
			else {
				// 射撃方向
				weapons_[i]->SetShotDirVelocity(Vector3{ 0,0,1 } * (model_.worldTF.rotation * weapons_[i]->GetWorldTF()->rotation));
				// 攻撃
				weapons_[i]->Attack(GameMask::player, GameMask::attack | GameMask::enemy);
				continue;
			}
		}
	}
}

void Actor::OnCollision(LWP::Object::Collision* hitTarget) {
	if (hitTarget->mask.GetHitFrag() != bodyCollision_.mask.GetBelongFrag()) { return; }
	// ダメージを受ける
	hp_->Damage(world_->FindAttackPower(hitTarget->name), hitTarget->name);
}

void Actor::ChangeState(StateBase* nextState) {
	// 同じ方なら終了
	if (state_ && typeid(*nextState) == typeid(*state_)) {
		delete nextState;
		return;
	}
	// 状態が生成されているときだけ解放
	if (state_) delete state_;
	state_ = nextState;
}

void Actor::SetWeapon(IWeapon* weapon, int weaponSide) {
	weapon->SetParent(this);
	weapons_[weaponSide] = weapon;
}
