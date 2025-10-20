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
	kMelee
};

/// <summary>
/// 武器の調整項目
/// </summary>
struct WeaponData {
	std::string modelName;			// 銃モデルのファイル名
	float shotIntervalTime;			// 射撃間隔[秒]
	float burstIntervalTime;		// バースト間隔[秒](バースト銃の場合に数値を入れる)
	float storeTime;				// 溜め時間[秒](溜める銃の場合に数値を入れる)
	float bulletNum;				// 弾数
	float bulletSpeed;				// 弾速
	float attackValue;				// 攻撃力
	float coolTime;					// リロード時間[秒]
};