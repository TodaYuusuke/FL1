#pragma once
#include <Adapter.h>
#include "BaseTutorialData.h"
#include "../../../Componets/Input/VirtualController.h"

class Player;
class EnemyManager;
class MoveTutorial : public BaseTutorialData {
public:
	enum class MoveGuideSequence {
		kMoveF,
		kMoveB,
		kMoveL,
		kMoveR,
		kCameraL,
		kCameraR,
		kTurn,
		kBoost,
		kCount
	};

public:
	// コンストラクタ
	MoveTutorial(Player* player, EnemyManager* enemyManager);
	// デストラクタ
	~MoveTutorial() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
	LWP::Math::Vector2 AdjustmentStick(LWP::Math::Vector2 stick);

private:
	VirtualController* vCon_;

private:
	// 
	std::map<MoveGuideSequence, LWP::Primitive::NormalSprite> guideFont_;

	// チュートリアルの順序
	MoveGuideSequence sequence_;

	// 方向ベクトル
	LWP::Math::Vector3 dir_;
	// 
	LWP::Math::Vector3 centerPos_;

	// 次のガイドに進めるかを
	bool isNextGuide_;
};

