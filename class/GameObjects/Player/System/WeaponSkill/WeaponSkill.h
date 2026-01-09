#pragma once
#include "../../../Weapon/WeaponConfig.h"
#include <Adapter.h>

/// <summary>
/// 武器の練度
/// </summary>
class WeaponSkill {
public:
	// 武器練度情報
	struct WeaponSkillData {
		float value;
		//float requiredExp;		// 必要経験値数
		int level;					// 練度
		float attackMultiply = 1.0f;// 攻撃倍率
		int weaponType;
	};
	// Guiのタブ情報
	struct TabData {
		std::string name;
		int level;
		int weaponType;
		bool isActive;
	};

	struct RaderChartData {
		float value;
		int level;
	};

public:
	// コンストラクタ
	WeaponSkill();
	// デストラクタ
	~WeaponSkill();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:
	void ApplyRadarEffect(std::array<WeaponSkillData, (int)WeaponType::kCount>& values, int index, float increase);

public:// アクセサ
	/// <summary>
	/// 武器の練度を上げる
	/// </summary>
	/// <param name="weaponType"></param>
	/// <param name="value"></param>
	void SkillUp(int weaponType, float value) {
		int type = GetSkillData(weaponType).weaponType;
		ApplyRadarEffect(radar_, type, value);
		//radar_[type].value += value;
	}
	//void SkillUp(const std::string& weaponName, float value) { 
	//	int type = GetSkillData(weaponName).weaponType;
	//	ApplyRadarEffect(radar_, type, value);
	//	//radar_[type].value += value;
	//}

#pragma region Getter
	/// <summary>
	/// 指定の武器練度情報を取得
	/// </summary>
	/// <param name="weaponType"></param>
	/// <returns></returns>
	WeaponSkillData GetSkillData(int weaponType) { 
		for (int i = 0; i < (int)WeaponType::kCount; i++) {
			if (radar_[i].weaponType == weaponType) return radar_[i];
		}
		return WeaponSkillData();
	}
	//WeaponSkillData GetSkillData(const std::string& weaponName) { 
	//	for (int i = 0; i < (int)WeaponType::kCount; i++) {
	//		if (radar_[i].weaponType == WeaponConfig::GetWeaponType(weaponName)) return radar_[i];
	//	}
	//	return WeaponSkillData();
	//}
	int GetIndex(int weaponType) {
		for (int i = 0; i < (int)WeaponType::kCount; i++) {
			if (radar_[i].weaponType == weaponType) return i;
		}
		return -1;
	}
#pragma endregion

#pragma region Setter

#pragma endregion

private:// 調整項目
	// 最大練度(実数値)
	float maxWeaponSkill = 3000.0f;
	float maxAttackMultiply = 2.0f;
	// 減衰割合
	float decay[4] = {
		0.0f,    // d=0 自分
		0.1f,   // d=1 隣
		0.2f,   // d=2
		0.4f    // d=3 反対側
	};

private:
	LWP::Utility::JsonIO json_;

	std::array<WeaponSkillData, (int)WeaponType::kCount> radar_;
};