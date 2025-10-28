#pragma once

// 移動系統の状態の種類
enum class MoveActionType {
	kMove,
	kJuup,
	kEvasion,
	kSliding,
	kCount
};
// 武器の状態の種類
enum class WeaponActionType {
	kDefault,
	kShot,
	kReloading,
	kADS,
	kCount
};

enum class ActionType {
	kMain,
	kSub,
	kCount
};

namespace ActionConfig {
	namespace Mask {
		// 移動状態
		namespace Movement {
			// メイン状態
			namespace MainAction {
				// 移動
				inline const unsigned int move = 0b01;

				// メイン状態すべて
				inline const unsigned int all = move;
			}
			// サブ状態
			namespace SubAction {
				// ジャンプ
				inline const unsigned int jump = 0b01 << 1;
				// スライディング
				inline const unsigned int sliding = 0b01 << 2;
				// 回避
				inline const unsigned int evasion = 0b01 << 3;

				// サブ状態すべて
				inline const unsigned int all = jump | sliding | evasion;
			}

			// 移動状態すべて
			inline const unsigned int all = MainAction::all | SubAction::all;
		}
		// 武器状態
		namespace Weapon {
			// メイン状態
			namespace MainAction {
				// 攻撃
				inline const unsigned int attack = 0b01 << 4;
				// リロード
				inline const unsigned int reloading = 0b01 << 5;

				// メイン状態すべて
				inline const unsigned int all = attack | reloading;
			}

			// 武器状態すべて
			inline const unsigned int all = MainAction::all;
		}

		// 全てのマスク
		inline const unsigned int all = Movement::all | Weapon::all;
	}
}