#pragma once
#include <Adapter.h>

// 弾の種類
// M...近接
// A...攻撃概念(直接呼ばないで)
enum class BulletType {
	LargeCaliber,		// 大口径
	SmallCaliber,		// 小口径
	BuckShot,			// ショットガンの弾
	Launcher,			// まっすぐに進む
	M_Pile,				// パイル近接攻撃
	kCount
};
// 攻撃着弾時の処理の種類
enum class ImpactType {
	kNomral,		// 通常
	kExplosion,		// 爆発
	kCount			
};
/// <summary>
/// 属性の種類
/// </summary>
enum class ElementType {
	Normal,				// 通常
	kCount
};

// 攻撃の調整項目
struct AttackData {
	int impactType;										// 着弾時の処理
	LWP::Math::Vector3 attackSize{ 1.0f,1.0f,1.0f };	// 攻撃の大きさ(当たり判定)
	float speed = 1.0f;									// 弾速
	float elapsedTime = 1.0f;							// 弾の生存時間
	float attackValue = 50.0f;							// 攻撃力
};
// 攻撃着弾時の処理の調整項目
struct ImpactData {
	std::string modelName;
	int type;
	float attackSize = 1.0f;							// 攻撃の大きさ(当たり判定)
	float speed = 1.0f;									// 弾速
	float elapsedTime = 1.0f;							// 弾の生存時間
	float attackValue = 50.0f;							// 攻撃力
};

namespace AttackConfig {
	// 属性
	namespace Element {
		// 識別名
		// 順番はBulletType準拠
		inline std::array<std::string, (int)ElementType::kCount> elementName = {
			"Normal"		// 通常
		};
	}
	// 弾(近接攻撃を含む)
	namespace Bullet {
		namespace Name {
			// 識別名
			// 順番はBulletType準拠
			inline std::array<std::string, (int)BulletType::kCount> bulletName = {
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
			for (int i = 0; i < Name::bulletName.size(); i++) {
				if (Name::bulletName[i] == name) {
					return i;
				}
			}
			// 該当なし
			return -1;
		}
	}
	// 攻撃着弾時
	namespace Impact {
		namespace Name {
			// 識別名
			// 順番はImpactType準拠
			inline std::array<std::string, (int)ImpactType::kCount> impactName = {
				"Normal",			// 通常
				"Explosion"			// 爆発
			};
		}
		//namespace ModelName {
		//	// 識別名
		//	// 順番はImpactType準拠
		//	inline std::array<std::string, (int)ImpactType::kCount> modelName = {
		//		"",					// 通常
		//		"Explosion"			// 爆発
		//	};
		//}

		/// <summary>
		/// 名前から攻撃着弾時の処理の種類を検索
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		inline int GetImpactType(const std::string& name) {
			for (int i = 0; i < Name::impactName.size(); i++) {
				if (Name::impactName[i] == name) {
					return i;
				}
			}
			// 該当なし
			return -1;
		}
	}
}