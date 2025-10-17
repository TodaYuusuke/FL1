#include "WeaponSlot.h"

void WeaponSlot::Init() {}

void WeaponSlot::Update() {
	// 更新
	if(!weapons_.empty()) weapons_.front()->Update();

	// 弾切れの武器を削除
	for (auto& w : weapons_) {
		if (!w) continue;

		// 削除コマンドがあるなら消す
		if (w->GetIsDestroy()) { w.reset(); }
	}

	// 前詰め
	Compact();
}

void WeaponSlot::DebugGui() {
	int i = 0;
	for (auto& w : weapons_) {
		i++;
		std::string label = "Slot" + std::to_string(i) + w->GetName();
		if (ImGui::TreeNode(label.c_str())) {
			w->DebugGui();
			ImGui::TreePop();
		}
	}
}

void WeaponSlot::Attack() {
	if (!weapons_.empty()) weapons_.front()->Attack();
}

void WeaponSlot::Compact() {
	// 武器
	weapons_.erase(
		std::remove(weapons_.begin(), weapons_.end(), nullptr),
		weapons_.end()
	);
	//// 武器UI
	//weaponUI_.erase(
	//	std::remove(weaponUI_.begin(), weaponUI_.end(), nullptr),
	//	weaponUI_.end()
	//);
}

void WeaponSlot::AddWeapon(std::unique_ptr<IWeapon> weapon) {
	if (weapons_.size() < kMaxWeapons) {
		weapons_.push_back(std::move(weapon));
	}
}