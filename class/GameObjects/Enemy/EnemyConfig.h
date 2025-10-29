#pragma once
#include "../Weapon/WeaponConfig.h"
#include <Adapter.h>

// 敵の種類
enum class EnemyType {
	kMelee,
	kGunner,
	kDrone,
	kTest,
	kCount
};

/// <summary>
/// 敵の調整データ
/// </summary>
struct EnemyData {
	// 使用する行動パターン(BehaviorTreeのファイル名)
	std::string BTFileName;
	// 種類
	int type = 0;
	// 所持する武器種と搭載位置
	// -1の時は何も持たない
	std::array<int, (int)WeaponSide::kCount> containWeaponTypes;
	// 武器レアリティの最低保証
	// -1は最低保証なし
	int minWeaponRarity = 0;
	// レベル
	int level = 1;
	// 攻撃倍率
	float attackMultiply = 1.0f;
	// 移動速度倍率
	float speedMultiply = 1.0f;
	// 体力
	float hp = 100.0f;

	EnemyData() {
		// 所持している武器をなしにする
		for (int i = 0; i < (int)WeaponSide::kCount; i++) {
			containWeaponTypes[i] = 0;
		}
	}
};

namespace EnemyConfig {
	namespace Name {
		// 名前
		// 順番はEnemyType準拠
		inline std::array<std::string, (int)EnemyType::kCount> name = {
			"Melee",
			"Gunner",
			"Drone",
			"Test"
		};
	}

	// ビヘイビアツリーのファイル名
	namespace BTFileName {
		// ビヘイビアツリーのファイル名
		// 順番はEnemyType準拠
		inline std::array<std::string, 3> fileName = {
			"resources/json/BT/BT_Melee.json",
			"resources/json/BT/BT_Gunner.json",
			"resources/json/BT/BT_Drone.json"
		};
	}

	// 識別名[変更禁止]
	inline std::string name = "Actor";
	// タグ名
	inline std::string tag = "Enemy";
}