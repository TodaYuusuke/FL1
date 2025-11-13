#pragma once
#include <Adapter.h>
#include <string>

// 武器種
enum class WeaponType {
	kMachineGun,
	kShotGun,
	kRifle,
	kLauncher,
	kMissile,
	kMelee,
	kCount
};
// レアリティ
enum class RarityType {
	kCommon,
	kUnCommon,
	kRare,
	kSuperRare,
	kLegendary,
	kOver,
	kCount
};

// 武器の装着箇所
enum class WeaponSide {
	kLeft,			// 左手
	kRight,			// 右手
	kLeftShoulder,	// 左肩
	kRightShoulder,	// 右肩
	kCount
};

// 発射スタイル
enum class ShotType {
	kFullAuto,	// フルオート
	kBurst,		// バースト
	kCount
};

/// <summary>
/// 武器の調整項目
/// </summary>
struct WeaponData {
	std::string name;							// 武器の名前
	std::string modelName;						// 銃モデルのファイル名
	float shotIntervalTime;						// 射撃間隔[秒]
	float burstIntervalTime;					// バースト間隔[秒](バースト銃の場合に数値を入れる)
	float storeTime;							// 溜め時間[秒](溜める銃の場合に数値を入れる)
	int burstNum;								// バースト数
	int sameBulletNum;							// 同時に出る弾数
	LWP::Math::Vector3 diffusingBulletRange;	// 弾の拡散範囲[0～1]
	int bulletNum;								// 弾数
	float bulletSpeed = 1.0f;					// 弾速
	float attackValue = 50.0f;					// 攻撃力
	float coolTime;								// 撃てない時間[秒]
	float reloadTime;							// リロード時間[秒]
	int rarity;									// レアリティ
};

namespace WeaponConfig {
	// 名前
	namespace Name {
		// 識別名
		// 順番はWeaponType準拠
		inline std::array<std::string, (int)WeaponType::kCount> name = {
			"MachineGun",	// マシンガン
			"ShotGun",		// ショットガン
			"Rifle",		// ライフル
			"Launcher",		// ランチャー
			"Missile",		// ミサイル
			"Melee"			// 近接
		};
	}
	// レアリティ
	namespace Rarity {
		// モデル名
		// 順番はRarityType準拠
		inline std::array<std::string, (int)RarityType::kCount> rarity = {
			"Common",
			"UnCommon",
			"Rare",
			"SuperRare",
			"Legendary",
			"Over"
		};
	}
	// 名前
	namespace ModelName {
		// モデル名
		// 順番はWeaponType準拠
		// Gun系→	resources/model/Gun/からのパス含めた名前
		// Melee系→resources/model/Melee/からのパス含めた名前
		inline std::array<std::array<std::string, (int)WeaponType::kCount>, (int)RarityType::kCount> modelName = {{
				  // Common						  UnCommon				   Rare				  SuperRare					 Legendary		Over
/* マシンガン	*/{  "MachineGun/MachineGun1.gltf"	, "MachineGun/MachineGun1.gltf"	, "MachineGun/MachineGun1.gltf"	,"MachineGun/MachineGun1.gltf"	, "MachineGun/MachineGun1.gltf"	,"MachineGun/MachineGun1.gltf"  },
/* ショットガン	*/{  "ShotGun/ShotGun1.gltf"		, "ShotGun/ShotGun1.gltf"		, "ShotGun/ShotGun1.gltf"		,"ShotGun/ShotGun1.gltf"		, "ShotGun/ShotGun1.gltf"		,"ShotGun/ShotGun1.gltf"		},
/* ライフル		*/{  "Rifle/Rifle.obj"				, "Rifle/Rifle.obj"				, "Rifle/Rifle.obj"				,"Rifle/Rifle.obj"				, "Rifle/Rifle.obj"				,"Rifle/Rifle.obj"				},
/* ランチャー	*/{  "Launcher/Launcher1.gltf"		, "Launcher/Launcher1.gltf"		, "Launcher/Launcher1.gltf"		,"Launcher/Launcher1.gltf"		, "Launcher/Launcher1.gltf"		,"Launcher/Launcher1.gltf"		},
/* ミサイル		*/{  "AR/AR.obj"					, "AR/AR.obj"					, "AR/AR.obj"					,"AR/AR.obj"					,"AR/AR.obj"					,"AR/AR.obj"					},
/* 近接			*/{  "AR.obj"						, "AR.obj"						, "AR.obj"						,"AR.obj"						,"AR.obj"						,"AR.obj"						}	
		}};
	}
}