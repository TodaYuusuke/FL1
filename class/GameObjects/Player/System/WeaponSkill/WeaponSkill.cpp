#include "WeaponSkill.h"
#include <array> 
#include <set>

WeaponSkill::WeaponSkill() {
	// 練度の詳細設定作成
	json_.Init("WeaponSkill.json");
	for (int i = 0; i < maxLevel; i++) {
		// レベル
		sampleSkills_[i].level = i;
		json_.BeginGroup("Level" + std::to_string(i))
			.AddValue<float>("RequiredExp", &sampleSkills_[i].requiredExp)
			.AddValue<float>("AttackMultiply", &sampleSkills_[i].attackMultiply)
			.EndGroup();
	}
	json_.CheckJsonFile();

	Init();
}

WeaponSkill::~WeaponSkill() {

}

void WeaponSkill::Init() {
	// 初期化
	for (int i = 0; i < (int)WeaponType::kCount; i++) {
		currentSkills_[i] = {
			.value = 0.0f,										// 経験値量
			.requiredExp = sampleSkills_[0].requiredExp,		// 必要経験値量
			.level = 0,											// 現在の練度
			.attackMultiply = sampleSkills_[0].attackMultiply	// 攻撃倍率
		};
	}
}

void WeaponSkill::Update() {
	// 練度を上げるかを判別
	CheckSkillUp();

	// 経験値量が下限上限を越さないようにする
	for (int i = 0; i < (int)WeaponType::kCount; i++) {
		int index = sampleSkills_.size() - 1;
		currentSkills_[i].value = std::clamp<float>(currentSkills_[i].value, 0.0f, sampleSkills_[index].requiredExp);
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

		static std::vector<TabData> tabs;
		// レベル文字列を保持するための配列
		static std::vector<std::string> levelStrings(maxLevel);
		static std::vector<const char*> itemLevels(maxLevel);
		static int currentLevel = 0;
		// 文字列を生成して寿命を確保
		for (int i = 0; i < maxLevel; i++) {
			levelStrings[i] = "Level_" + std::to_string(sampleSkills_[i].level);
			itemLevels[i] = levelStrings[i].c_str();
		}
		// タブの選択
		if (ImGui::Combo("skill level", &currentLevel, itemLevels.data(), itemLevels.size())) {
			bool isPushBack = true;
			for (int i = 0; i < tabs.size(); ++i) {
				// 重複してたらプッシュしない
				if (tabs[i].level == currentLevel) {
					isPushBack = false;
					break;
				}
			}
			if (isPushBack)tabs.push_back(TabData{ levelStrings[currentLevel], currentLevel, -1, true });
		}
		// タブの更新
		if (ImGui::BeginTabBar("ModifyTabs", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll)) {
			for (int i = 0; i < tabs.size(); ++i) {
				if (ImGui::BeginTabItem(tabs[i].name.c_str(), &tabs[i].isActive, ImGuiTabBarFlags_Reorderable)) {
					ImGui::DragFloat("RequiredExp", &sampleSkills_[tabs[i].level].requiredExp);
					ImGui::DragFloat("AttackMultiply", &sampleSkills_[tabs[i].level].attackMultiply);
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		// erase-remove
		tabs.erase(
			std::remove_if(tabs.begin(), tabs.end(),
				[](const TabData& tab) { return !tab.isActive; }),
			tabs.end()
		);
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
			if(isPushBack) tabs.push_back(TabData{ items[currentWeapon], -1, currentWeapon, true });
		}
		if (ImGui::BeginTabBar("MainTabs", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll)) {
			for (int i = 0; i < tabs.size(); ++i) {
				if (ImGui::BeginTabItem(tabs[i].name.c_str(), &tabs[i].isActive, ImGuiTabBarFlags_Reorderable)) {
					ImGui::DragInt("Level", &currentSkills_[tabs[i].weaponType].level);
					ImGui::DragFloat("Exp", &currentSkills_[tabs[i].weaponType].value);
					ImGui::DragFloat("EequiredExp", &currentSkills_[tabs[i].weaponType].requiredExp);
					ImGui::DragFloat("AttackMultiply", &currentSkills_[tabs[i].weaponType].attackMultiply);
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

void WeaponSkill::CheckSkillUp() {
	for (int i = 0; i < (int)WeaponType::kCount; i++) {
		// 最大練度なら処理しない
		if (currentSkills_[i].level >= maxLevel - 1) { continue; }

		// 現在の練度
		float level = currentSkills_[i].level;
		// 必要経験値を達しているか
		if (sampleSkills_[level].requiredExp <= currentSkills_[i].value) {
			currentSkills_[i].level++;
			currentSkills_[i].attackMultiply = sampleSkills_[level + 1].attackMultiply;
			currentSkills_[i].requiredExp = sampleSkills_[level + 1].requiredExp;
		}
	}
}

void WeaponSkill::SelectLevel(int& selectedLevel, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!sampleSkills_.empty()) {
		std::string combo_preview_value = "Level : " + std::to_string(sampleSkills_[selectedLevel].level);
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value.c_str())) {
			for (int n = 0; n < sampleSkills_.size(); n++) {
				const bool is_selected = ((int)selectedLevel == n);
				std::string selectableLabel = "Level : " + std::to_string(sampleSkills_[n].level);
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedLevel = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found level"));
	}
}
