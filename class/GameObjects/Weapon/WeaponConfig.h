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

/// <summary>
/// 武器の調整項目
/// </summary>
struct WeaponData {
	std::string name;				// 武器の名前
	std::string modelName;			// 銃モデルのファイル名
	float shotIntervalTime;			// 射撃間隔[秒]
	float burstIntervalTime;		// バースト間隔[秒](バースト銃の場合に数値を入れる)
	float storeTime;				// 溜め時間[秒](溜める銃の場合に数値を入れる)
	float bulletNum;				// 弾数
	float bulletSpeed = 1.0f;		// 弾速
	float attackValue;				// 攻撃力
	float coolTime;					// リロード時間[秒]
	int rarity;						// レアリティ
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
			// マシンガン	ショットガン		 ライフル	  ランチャー   ミサイル	    近接
			{  "AR/AR.obj",	"ShotGun/Rifle.obj", "AR/AR.obj", "AR/AR.obj", "AR/AR.obj",	"AR.obj"  },	// Common
			{  "AR/AR.obj",	"ShotGun/Rifle.obj", "AR/AR.obj", "AR/AR.obj", "AR/AR.obj",	"AR.obj"  },	// UnCommon
			{  "AR/AR.obj",	"ShotGun/Rifle.obj", "AR/AR.obj", "AR/AR.obj", "AR/AR.obj",	"AR.obj"  },	// Rare
			{  "AR/AR.obj",	"ShotGun/Rifle.obj", "AR/AR.obj", "AR/AR.obj", "AR/AR.obj",	"AR.obj"  },	// SuperRare
			{  "AR/AR.obj",	"ShotGun/Rifle.obj", "AR/AR.obj", "AR/AR.obj", "AR/AR.obj",	"AR.obj"  },	// Legengary
			{  "AR/AR.obj",	"ShotGun/Rifle.obj", "AR/AR.obj", "AR/AR.obj", "AR/AR.obj",	"AR.obj"  }		// Over
		}};
	}
}