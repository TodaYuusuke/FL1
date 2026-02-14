#pragma once
#include <Adapter.h>
#include "TutorialData/BaseTutorialData.h"
#include "../../Componets/Input/VirtualController.h"

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

private:
	/// <summary>
	/// 終了コマンド
	/// </summary>
	void FinishCommand();

public:
	bool GetIsFinish() { return isFinish_; }

private:// 定数
	// チュートリアル終了に必要なコマンド長押し時間
	const float pressFisnishCommandFrame = 120.0f;

private:// 外部から受け取る変数
	Player* player_;
	EnemyManager* enemyManager_;

private:
	// チュートリアルの種類
	std::unique_ptr<BaseTutorialData> guideData_;
	// チュートリアルの進捗状況
	GuideSequence sequence_;

	// チュートリアル終了に必要なコマンド
	std::vector<BindActionType> finishCommands_;
	// 終了コマンドUI
	std::map<std::string, LWP::Primitive::ClipSprite> finishSprites_;

	int currentGuideNum_;

	// 終了コマンドの押している時間
	float finishFrame_;

	bool isFinish_;
};