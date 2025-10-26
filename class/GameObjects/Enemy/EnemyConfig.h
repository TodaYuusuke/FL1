#pragma once
#include <Adapter.h>

namespace EnemyConfig {
	// 敵の種類
	enum class EnemyType {
		kMelee,
		kGunner,
		kDrone,
		kTest,
		kCount
	};

	namespace Name {
		// 名前
		// 順番はEnemyType準拠
		inline std::array<std::string, 4> name = {
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