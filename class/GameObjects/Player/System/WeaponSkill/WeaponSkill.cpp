#include "WeaponSkill.h"
#include <array> 
#include <algorithm>
#include <cmath>
#include <set>

WeaponSkill::WeaponSkill() {
	radar_[0].weaponType = (int)WeaponType::kRifle;
	radar_[1].weaponType = (int)WeaponType::kMachineGun;
	radar_[2].weaponType = (int)WeaponType::kShotGun;
	radar_[3].weaponType = (int)WeaponType::kMelee;
	radar_[4].weaponType = (int)WeaponType::kMissile;
	radar_[5].weaponType = (int)WeaponType::kLauncher;

	// 練度の詳細設定作成
	json_.Init("WeaponSkill.json");
	// 減衰率
	json_.BeginGroup("Decay")
		.AddValue<float>("Near", &decay[1])
		.AddValue<float>("Middle", &decay[2])
		.AddValue<float>("Far", &decay[3])
		.EndGroup()
		.AddValue<float>("MaxWeaponSkill", &maxWeaponSkill)
		.AddValue<float>("MaxAttackMultiply", &maxAttackMultiply)
		.CheckJsonFile();

	Init();
}

WeaponSkill::~WeaponSkill() {

}

void WeaponSkill::Init() {
	// 初期化
	for (int i = 0; i < (int)WeaponType::kCount; i++) {
		radar_[i].value = 0.0f;				// 経験値量
		radar_[i].level = 0;				// 現在の練度
		radar_[i].attackMultiply = 1.0f;	// 攻撃倍率
	}
}

void WeaponSkill::Update() {
	// 経験値量が下限上限を越さないようにする
	for (int i = 0; i < (int)WeaponType::kCount; i++) {
		radar_[i].value = std::clamp<float>(radar_[i].value, 0.0f, maxWeaponSkill);
	}

	// 経験値から攻撃倍率を算出
	for (int j = 0; j < (int)WeaponType::kCount; j++) {
		// 攻撃倍率
		float attackMultiply = 1.0f + (radar_[j].value / maxWeaponSkill) * (maxAttackMultiply - 1.0f);
		radar_[j].attackMultiply = attackMultiply;
	}
}

void WeaponSkill::DebugGui() {
	if (ImGui::TreeNode("Json")) {
		if (ImGui::Button("Save")) {
			json_.Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			json_.Load();
		}
		// 減衰率
		if (ImGui::TreeNode("Decay")) {
			ImGui::DragFloat("Near", &decay[1], 0.01f);
			ImGui::DragFloat("Middle", &decay[2], 0.01f);
			ImGui::DragFloat("Far", &decay[3], 0.01f);
			ImGui::TreePop();
		}

		ImGui::DragFloat("MaxWeaponSkill", &maxWeaponSkill, 0.01f);
		ImGui::DragFloat("MaxAttackMultiply", &maxAttackMultiply, 0.01f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("CurrentData")) {
		static std::vector<TabData> tabs;
		const char* items[(int)WeaponType::kCount];
		for (int i = 0; i < (int)WeaponType::kCount; i++) {
			items[i] = WeaponConfig::Name::name[i].c_str();
		}
		static int currentWeapon = 0;
		if (ImGui::Combo("Weapon Skill", &currentWeapon, items, IM_ARRAYSIZE(items))) {
			bool isPushBack = true;
			for (int i = 0; i < tabs.size(); ++i) {
				// 重複してたらプッシュしない
				if (tabs[i].name == items[currentWeapon]) {
					isPushBack = false;
					break;
				}
			}
			// 新しいタブを追加
			if (isPushBack) tabs.push_back(TabData{ items[currentWeapon], -1, currentWeapon, true });
		}
		if (ImGui::BeginTabBar("MainTabs", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll)) {
			for (int i = 0; i < tabs.size(); ++i) {
				if (ImGui::BeginTabItem(tabs[i].name.c_str(), &tabs[i].isActive, ImGuiTabBarFlags_Reorderable)) {
					int weaponType = GetIndex(i);
					ImGui::DragInt("Level", &radar_[weaponType].level);
					ImGui::DragFloat("Exp", &radar_[weaponType].value);
					ImGui::DragFloat("AttackMultiply", &radar_[weaponType].attackMultiply);
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
		// タブの"閉じる"を押されたものを削除
		tabs.erase(
			std::remove_if(tabs.begin(), tabs.end(),
				[](const TabData& tab) { return !tab.isActive; }),
			tabs.end()
		);
		ImGui::TreePop();
	}
}

void WeaponSkill::ApplyRadarEffect(std::array<WeaponSkillData, (int)WeaponType::kCount>& values, int index, float increase) {
	int target = GetIndex(index);

	// 中心項目を上げる
	values[target].value += increase;

	for (int j = 0; j < (int)WeaponType::kCount; j++) {
		if (j == target) continue;

		// 円環距離
		int diff = std::abs(j - target);
		int d = std::min(diff, (int)WeaponType::kCount - diff);  // 0~3
		// 経験値を減らす
		values[j].value += increase * -decay[d];

		// 攻撃倍率
		float attackMultiply = 1.0f + (values[j].value / maxWeaponSkill) * (maxAttackMultiply - 1.0f);
		values[j].attackMultiply = attackMultiply;
	}
}