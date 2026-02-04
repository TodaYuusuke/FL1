#include "WeaponSlot.h"
#include "../Player/System/LeadingSystem.h"
#include "../Player/System/WeaponSkill/WeaponSkill.h"
#include "../Weapon/WeaponManager.h"
#include "../Collision/CollisionMask.h"

using namespace LWP::Math;
using namespace FLMath;

WeaponSlot::WeaponSlot(LeadingSystem* leadingSystem, WeaponSkill* weaponSkill, Actor* target, int weaponSide)
{
	pLeadingSystem_ = leadingSystem;
	pWeaponSkill_ = weaponSkill;
	target_ = target;
	weaponSide_ = weaponSide;
}

WeaponSlot::~WeaponSlot() {}

void WeaponSlot::Init() {}

void WeaponSlot::Update() {
	// 練度を踏まえて武器の攻撃倍率更新
	if (!weapons_.empty()) weapons_.front()->SetAttackMultiply(pWeaponSkill_->GetSkillData(weapons_.front()->GetWeaponData().type).attackMultiply);

	// 削除コマンドがあるなら消す
	weapons_.erase(
		std::remove_if(weapons_.begin(), weapons_.end(),
			[&](IWeapon* w) {
				if (w->GetIsDestroy()) {
					w->Destroy();
					// 武器管理クラスに該当武器の解放依頼
					WeaponManager::GetInstance()->DeleteWeapon(w);
					return true; // vectorから削除対象
				}
				return false;
			}),
		weapons_.end()
	);

	// 前詰め
	Compact();
}

void WeaponSlot::DebugGui() {
	int i = 0;
	for (IWeapon* w : weapons_) {
		i++;
		std::string label = "Slot" + std::to_string(i) + ": " + w->GetName();
		if (ImGui::TreeNode(label.c_str())) {
			w->DebugGui();
			ImGui::TreePop();
		}
	}
}

void WeaponSlot::Attack() {
	// 武器がないなら処理しない
	if (weapons_.empty()) { return; }
	// 攻撃対象
	Actor* actor = nullptr;

	// 弾がある状態か
	if (!weapons_.front()->GetIsEmpty()) {
		// 攻撃できる状態か
		if (weapons_.front()->GetIsEnableAttack()) {
			// 攻撃時に練度を上げる
			pWeaponSkill_->SkillUp(weapons_.front()->GetWeaponData().type, weapons_.front()->GetWeaponData().attackSkillGain);

			// 偏差射撃
			Vector3 shotVel = pLeadingSystem_->GetLeadingShotAngle(weapons_.front()->GetWorldTF()->GetWorldPosition(), 1.0f);
			if (Vector3::Dot(shotVel, shotVel) != 0.0f) {
				weapons_.front()->SetShotDirVelocity(shotVel);
			}
			else {
				weapons_.front()->SetShotDirVelocity(GetDirVector({ 0,0,1 }, weapons_.front()->GetActor()->GetModel()->worldTF.rotation));
			}

			// 相手がだれかを指定するために更新処理一回呼ぶ
			pLeadingSystem_->Update();
			actor = pLeadingSystem_->GetLeadingTarget();
		}
	}

	// 攻撃
	weapons_.front()->Attack(GameMask::enemy, GameMask::attack | GameMask::player, actor);

	// 攻撃中なら適したアニメーションを再生
	if(weapons_.front()->GetIsAttacking()) {
		target_->PlayAttackAnim(weaponSide_);
	}
}

void WeaponSlot::Compact() {
	// 武器
	weapons_.erase(
		std::remove(weapons_.begin(), weapons_.end(), nullptr),
		weapons_.end()
	);
}

void WeaponSlot::WeaponVelUpdate() {

}

void WeaponSlot::AddWeapon(IWeapon* weapon) {
	if (weapons_.size() < kMaxWeapons) {
		weapons_.push_back(weapon);
	}
}

void WeaponSlot::DeleteWeapons() {
	// 削除コマンドがあるなら消す
	weapons_.erase(
		std::remove_if(weapons_.begin(), weapons_.end(),
			[&](IWeapon* w) {
				// 武器管理クラスに該当武器の解放依頼
				WeaponManager::GetInstance()->DeleteWeapon(w);
				return true; // vectorから削除対象
			}),
		weapons_.end()
	);
}