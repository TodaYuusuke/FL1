#pragma once
#include <Adapter.h>

enum class BindActionType {
	kCollect,			// 
	kLeftHand,			// 
	kRightHand,			// 
	kLeftShoulder,		// 
	kRightShoulder,		// 
	kBoost,				// 加速
	kTurn,				// 180度回転
	kInteract,			// 決定ボタン
	kCancel,			// キャンセルボタン
	kCount
};

namespace InputConfig {
	namespace Command {
		// キーボード
		namespace Key {
			// 移動
			namespace Move {
				inline uint8_t Up = DIK_W;
				inline uint8_t Down = DIK_S;
				inline uint8_t Left = DIK_A;
				inline uint8_t Right = DIK_D;
			};
			// 視点移動
			namespace CameraMove {
				inline uint8_t Up = DIK_UP;
				inline uint8_t Down = DIK_DOWN;
				inline uint8_t Left = DIK_LEFT;
				inline uint8_t Right = DIK_RIGHT;
			};
			inline int collect			= DIK_LSHIFT;		// 武器回収
			inline int leftShoulder		= DIK_3;			// 左肩武器使用
			inline int leftHand			= DIK_1;			// 左手武器使用
			inline int rightShoulder	= DIK_4;			// 右肩武器使用
			inline int rightHand		= DIK_2;			// 右手武器使用
			inline int boost			= DIK_SPACE;		// 加速
			inline int turn				= DIK_LCONTROL;		// 180度回転
			inline int interact			= DIK_F;			// 決定ボタン
			inline int cancel			= DIK_ESCAPE;		// キャンセルボタン
		}

		// ゲームパッド
		namespace GamePad {
			inline int collect			= XBOX_X;			// 武器回収
			inline int leftShoulder		= XBOX_LB;			// 左肩武器使用
			inline int leftHand			= XBOX_LT;			// 左手武器使用
			inline int rightShoulder	= XBOX_RB;			// 右肩武器使用
			inline int rightHand		= XBOX_RT;			// 右手武器使用
			inline int boost			= XBOX_A;			// 加速
			inline int turn				= XBOX_B;			// 180度回転
			inline int interact			= XBOX_A;			// 決定ボタン
			inline int cancel			= XBOX_B;			// キャンセルボタン
		}
	}
}