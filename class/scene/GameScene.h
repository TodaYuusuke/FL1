#pragma once
#include "scene/IScene.h" 
#include "../GameObjects/World/World.h"
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/Enemy/EnemyManager.h"
#include "../GameObjects/Camera/FollowCamera/FollowCamera.h"
#include "../GameObjects/UI/ScoreUI/ScoreUI.h"
#include "Adapter.h"

class GameScene final
	: public IScene {
public: // ** 純粋仮想関数の実体宣言 ** //
	GameScene();
	~GameScene();

	// 初期化
	void Initialize();
	// 更新
	void Update();

private: // 調整項目
	// クリアまでに必要なキル数
	int clearKillCount = 30;

	LWP::Utility::JsonIO json_;

private:
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// 敵管理クラス
	std::unique_ptr<EnemyManager> enemyManager_;

	Player* player_;

	// ワールド
	std::unique_ptr<World> world_;

	std::unique_ptr<ScoreUI> score_;
};