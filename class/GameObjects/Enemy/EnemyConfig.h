#pragma once
#include "../Weapon/WeaponConfig.h"
#include <Adapter.h>

// 敵の種類
enum class EnemyType {
	kMelee,
	kGunner,
	kDrone,
	kCargo,
	kTest,
	kCount
};

/// <summary>
/// 出現方式
/// </summary>
enum class EnemySpawnType {
	kNone,
	kRandom,	// ランダム
	kWave,		// ウェーブ制
	kCount		// 
};

/// <summary>
/// 敵のレベル情報
/// </summary>
struct LevelParameter {
	// レベル
	int value = 1;
	// 攻撃倍率
	float attackMultiply = 1.0f;
	// 移動速度倍率
	float speedMultiply = 1.0f;
};

/// <summary>
/// 敵の調整データ
/// </summary>
struct EnemyData {
	// 使用する行動パターン(BehaviorTreeのファイル名)
	std::string BTFileName;
	// モデル名
	std::string modelName;

	// 当たり判定のmin
	LWP::Math::Vector3 colliderMin = { -0.5f, -0.5f, -0.5f };
	// 当たり判定のmax
	LWP::Math::Vector3 colliderMax = { 0.5f, 0.5f, 0.5f };

	// 種類
	int type = 0;
	// 所持する武器種と搭載位置
	// -1の時は何も持たない
	std::array<int, (int)WeaponSide::kCount> containWeaponTypes;
	std::array<int, (int)WeaponSide::kCount> containWeaponNames;
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

	// 得点
	float score;

	EnemyData() {
		// 所持している武器をなしにする
		for (int i = 0; i < (int)WeaponSide::kCount; i++) {
			containWeaponTypes[i] = 0;
		}
	}
};

/// <summary>
/// 敵配置時に必要な情報(エディタ用)
/// </summary>
struct EnemySpawnData {
	bool isSpawn;
	int type;
	int id;
	float spawnTime;
	LWP::Math::Vector3 pos;
	LWP::Resource::RigidModel debugModel;
};

struct SpawnEffectData {
	float currentFrame;
	LWP::Math::Vector3 pos;
	std::array<LWP::Resource::RigidModel, 2> spawnEffectModels;
};

namespace EnemyConfig {
	namespace Name {
		// 名前
		// 順番はEnemyType準拠
		inline std::array<std::string, (int)EnemyType::kCount> name = {
			"Melee",
			"Gunner",
			"Drone",
			"Cargo",
			"Test"
		};
	}

	// ビヘイビアツリーのファイル名
	namespace BTFileName {
		// ビヘイビアツリーのファイル名
		// 順番はEnemyType準拠
		inline std::array<std::string, (int)EnemyType::kCount - 1> fileName = {
			"resources/json/BT/BT_Melee.json",
			"resources/json/BT/BT_Gunner.json",
			"resources/json/BT/BT_Drone.json",
			"resources/json/BT/BT_Cargo.json"
		};

		/// <summary>
		/// ビヘイビアツリーのファイル名から要素番号を検索
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		inline int GetBTNameIndex(const std::string& name) {
			for (int i = 0; i < fileName.size(); i++) {
				if (fileName[i] == name) {
					return i;
				}
			}
			// 該当なし
			return -1;
		}
	}

	// モデル名前
	namespace ModelName {
		// モデル名
		// 順番はEnemyType準拠
		inline std::array<std::string, (int)EnemyType::kCount> modelName = {
			"resources/model/Enemy/Melee/Player.gltf",			// 近距離
			"resources/model/Enemy/Gunner/Player.gltf",			// 遠距離
			"resources/model/Enemy/Drone/Drone.gltf",			// ドローン
			"resources/model/Enemy/Cargo/HoverTank.gltf",		// 輸送機
			"resources/model/Enemy/Melee/Player.gltf"			// テスト敵
		};
	}

	// 識別名[変更禁止]
	inline std::string name = "Actor";
	// タグ名
	inline std::string tag = "Enemy";
}