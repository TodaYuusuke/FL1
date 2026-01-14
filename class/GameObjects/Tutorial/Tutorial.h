#pragma once
#include <Adapter.h>
#include "TutorialData/BaseTutorialData.h"

class Player;
class EnemyManager;
/// <summary>
/// チュートリアルクラス
/// </summary>
class Tutorial {
public:
	enum class GuideSequence {
		kMove,
		kCollect,
		kAttack,
		kCount
	};

public:
	// コンストラクタ
	Tutorial(Player* player, EnemyManager* enemyManager);
	// デストラクタ
	~Tutorial();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:
	bool GetIsFinish() { return isFinish_; }

private:// 外部から受け取る変数
	Player* player_;
	EnemyManager* enemyManager_;

private:
	//std::vector<BaseTutorialData*> guideData_;
	std::unique_ptr<BaseTutorialData> guideData_;

	GuideSequence sequence_;

	int currentGuideNum_;

	bool isFinish_;
};