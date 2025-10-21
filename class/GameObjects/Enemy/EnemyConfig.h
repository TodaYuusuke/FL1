#pragma once
#include <Adapter.h>

namespace EnemyConfig {
	// 敵の種類
	enum EnemyType {
		kMelee,
		kGunner,
		kDrone,
		kTest,
		kCount
	};

	// 識別名[変更禁止]
	inline std::string name = "Actor";
	// タグ名
	inline std::string tag = "Enemy";
}