#include "WeaponSlot.h"
#include "../Player/System/LeadingSystem.h"
#include "../Weapon/WeaponManager.h"
#include "../Collision/CollisionMask.h"

using namespace LWP::Math;
using namespace FLMath;

WeaponSlot::WeaponSlot(LeadingSystem* leadingSystem) {
	pLeadingSystem_ = leadingSystem;
}

WeaponSlot::~WeaponSlot() {}

void WeaponSlot::Init() {}

void WeaponSlot::Update() {
	// 更新
	if (!weapons_.empty()) weapons_.front()->Update();

	// 削除コマンドがあるなら消す
	weapons_.erase(
		std::remove_if(weapons_.begin(), weapons_.end(),
			[&](IWeapon* w) {
				if (w->GetIsDestroy()) {
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
	if (!weapons_.empty()) {
		// 弾がない状態なら撃てない
		if (!weapons_.front()->GetIsEmpty()) {
			// 射撃できる状態か
			if (weapons_.front()->GetIsEnableAttack()) {
				Vector3 shotVel = pLeadingSystem_->GetLeadingShotAngle(weapons_.front()->GetWorldTF()->GetWorldPosition(), 1.0f);

				if (Vector3::Dot(shotVel, shotVel) != 0.0f) {
					weapons_.front()->SetShotDirVelocity(shotVel);
				}
				else {
					weapons_.front()->SetShotDirVelocity(GetDirVector({ 0,0,1 }, weapons_.front()->GetActor()->GetModel().worldTF.rotation));
				}
			}
		}

		// 近接武器なら相手を指定
		Actor* actor = nullptr;
		if (weapons_.front()->GetWeaponData().name == WeaponConfig::Name::name[(int)WeaponType::kMelee]) {
			pLeadingSystem_->Update();
			actor = pLeadingSystem_->GetLeadingTarget();
		}

		weapons_.front()->Attack(GameMask::enemy, actor);
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