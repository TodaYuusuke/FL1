#pragma once
#include <Adapter.h>

// 弾の種類
// M...近接
enum class BulletType {
	LargeCaliber,		// 大口径
	SmallCaliber,		// 小口径
	BuckShot,			// ショットガンの弾
	Launcher,			// まっすぐに進む
	M_Pile,
	kCount
};

// 弾の調整項目
struct BulletData {
	LWP::Math::Vector3 bulletSize{ 1.0f,1.0f,1.0f };	// 弾の大きさ(当たり判定)
	float speed = 1.0f;									// 弾速
	float elapsedTime = 1.0f;							// 弾の生存時間
	float attackValue = 50.0f;							// 攻撃力
};

namespace BulletConfig {
	namespace Name {
		// 識別名
		// 順番はBulletType準拠
		inline std::array<std::string, (int)BulletType::kCount> name = {
			"LargeCaliber",		// 大口径
			"SmallCaliber",		// 小口径
			"BuckShot",			// 散弾
			"Launcher",			// ランチャー弾
			"M_Pile"			// 近接攻撃
		};
	}

	/// <summary>
	/// 名前から武器の種類を検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	inline int GetBulletType(const std::string& name) {
		for (int i = 0; i < Name::name.size(); i++) {
			if (Name::name[i] == name) {
				return i;
			}
		}
		// 該当なし
		return -1;
	}
}