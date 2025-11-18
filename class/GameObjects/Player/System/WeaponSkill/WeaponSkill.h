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
		float requiredExp;			// 必要経験値数
		int level;					// 練度
		float attackMultiply = 1.0f;// 攻撃倍率
	};
	// Guiのタブ情報
	struct TabData {
		std::string name;
		int level;
		int weaponType;
		bool isActive;
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
	/// <summary>
	/// 練度をあげるかを確認
	/// </summary>
	void CheckSkillUp();

	void SelectLevel(int& selectedLevel, std::string label);

public:// アクセサ
	/// <summary>
	/// 武器の練度を上げる
	/// </summary>
	/// <param name="weaponType"></param>
	/// <param name="value"></param>
	void SkillUp(int weaponType, float value) { currentSkills_[weaponType].value += value; }
	void SkillUp(const std::string& weaponName, float value) { currentSkills_[WeaponConfig::GetWeaponType(weaponName)].value += value; }

#pragma region Getter
	/// <summary>
	/// 指定の武器練度情報を取得
	/// </summary>
	/// <param name="weaponType"></param>
	/// <returns></returns>
	WeaponSkillData GetSkillData(int weaponType) { return currentSkills_[weaponType]; }
	WeaponSkillData GetSkillData(const std::string& weaponName) { return currentSkills_[WeaponConfig::GetWeaponType(weaponName)]; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:// 調整項目
	// 最大練度
	static const int maxLevel = 10;

	// 練度ごとのパラメータ
	std::array<WeaponSkillData, maxLevel> sampleSkills_;

private:
	LWP::Utility::JsonIO json_;

	// 武器別練度
	// 順番はWeaponType準拠
	std::array<WeaponSkillData, (int)WeaponType::kCount> currentSkills_;
};