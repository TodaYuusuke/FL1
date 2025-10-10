#pragma once
#include <Adapter.h>
#include <string>

/// <summary>
/// 銃の調整項目
/// </summary>
struct GunData {
	std::string gunModelName;		// 銃モデルのファイル名
	std::string magazineModelName;	// マガジンモデルのファイル名
	float shotIntervalTime;			// 射撃間隔[秒]
	float burstIntervalTime;		// バースト間隔[秒](バースト銃の場合に数値を入れる)
	float storeTime;				// 溜め時間[秒](溜める銃の場合に数値を入れる)
	float bulletNum;				// 弾数
	float attackValue;				// 攻撃力
	float reloadTime;				// リロード時間[秒]
};