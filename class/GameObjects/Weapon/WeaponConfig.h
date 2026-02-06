#pragma once
#include <Adapter.h>
#include <string>
#include <array>

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
	std::string name="";							// 武器の名前
	std::string modelName="";						// 武器モデルのファイル名
	std::string texName="";							// 武器モデルのテクスチャ名
	std::string animName="";						// 武器モデルのアニメーション名
	std::string jsonFileName="";					// jsonファイル名
	std::string attackSEFileName="";				// 効果音ファイル名
	std::string attackEffectName="";				// 攻撃時の演出名
	float shotIntervalTime;							// 射撃間隔[秒]
	float burstIntervalTime;						// バースト間隔[秒](バースト銃の場合に数値を入れる)
	float storeTime;								// 溜め時間[秒](溜める銃の場合に数値を入れる)
	int burstNum;									// バースト数
	int sameBulletNum;								// 同時に出る弾数
	int bulletType;									// 弾の種類
	LWP::Math::Vector3 diffusingBulletRange{};		// 弾の拡散範囲[0～1]
	int bulletNum;									// 弾数
	float coolTime;									// 撃てない時間[秒]
	float reloadTime;								// リロード時間[秒]
	float attackSkillGain = 100.0f;					// 攻撃時の練度上昇量
	int rarity;										// レアリティ
	int type;
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
				  // Common							UnCommon						Rare							SuperRare						Legendary						Over
/* マシンガン	*/{  "MachineGun/MachineGun.gltf"	, "MachineGun/MachineGun.gltf"	, "MachineGun/MachineGun.gltf"	,"MachineGun/MachineGun.gltf"	,"MachineGun/MachineGun.gltf"	,"MachineGun/MachineGun.gltf"	},
/* ショットガン	*/{  "ShotGun/ShotGun.gltf"			, "ShotGun/ShotGun.gltf"		, "ShotGun/ShotGun.gltf"		,"ShotGun/ShotGun.gltf"			,"ShotGun/ShotGun.gltf"			,"ShotGun/ShotGun.gltf"			},
/* ライフル		*/{  "Rifle/Rifle.obj"				, "Rifle/Rifle.obj"				, "Rifle/Rifle.obj"				,"Rifle/Rifle.obj"				,"Rifle/Rifle.obj"				,"Rifle/Rifle.obj"				},
/* ランチャー	*/{  "Launcher/Launcher.gltf"		, "Launcher/Launcher.gltf"		, "Launcher/Launcher.gltf"		,"Launcher/Launcher.gltf"		,"Launcher/Launcher.gltf"		,"Launcher/Launcher.gltf"		},
/* ミサイル		*/{  "Missle/MisslePod.obj"			, "Missle/MisslePod.obj"		, "Missle/MisslePod.obj"		,"Missle/MisslePod.obj"			,"Missle/MisslePod.obj"			,"Missle/MisslePod.obj"			},
/* 近接			*/{  "PileBunker.gltf"				, "PileBunker.gltf"				, "PileBunker.gltf"				,"PileBunker.gltf"				,"PileBunker.gltf"				,"PileBunker.gltf"				}	
		}};
	}
	// テクスチャ名
	namespace TextureName {
		// 武器のUI
		// 順序はWeaponType準拠
		namespace UI {
			inline std::array<std::string, (int)WeaponType::kCount> uiName = {
				"Weapon/machingun_UI.png",	// マシンガン
				"Weapon/shotgun_UI.png",		// ショットガン
				"Weapon/rifle_UI.png",	// ライフル
				"Weapon/launcher_UI.png",		// ランチャー
				"Weapon/missile_UI.png",	// ミサイル
				"Weapon/pilebunker_UI.png"		// 近接
			};
		}

		namespace UIGauge {
			inline std::array<std::string, (int)WeaponType::kCount> uiName = {
				"Weapon/machingun_Gauge_UI.png",	// マシンガン
				"Weapon/shotgun_Gauge_UI.png",		// ショットガン
				"Weapon/rifle_Gauge_UI.png",	// ライフル
				"Weapon/launcher_Gauge_UI.png",		// ランチャー
				"Weapon/missile_Gauge_UI.png",	// ミサイル
				"Weapon/pilebunker_Gauge_UI.png"		// 近接
			};
		}

		// 落ちてる武器の光柱
		// 順序はWeaponType準拠
		namespace LightPillar {
			namespace Color {
				// (中間プレイ会専用)
				namespace Weapon {
					namespace Rarity {
						inline std::array<unsigned int, (int)RarityType::kCount> color = {
							LWP::Utility::WHITE,		// Common
							LWP::Utility::GREEN,		// UnCommon
							LWP::Utility::BLUE,			// Rare
							LWP::Utility::CYAN,			// SuperRare
							LWP::Utility::MAGENTA,		// Legendary
							LWP::Utility::RED			// Over
						};
					}
				}
			}
		}
	}
}