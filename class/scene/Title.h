#pragma once
#include "scene/IScene.h"
#include "../GameObjects/World/World.h"
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/Enemy/EnemyManager.h"
#include "../GameObjects/Camera/FollowCamera/FollowCamera.h"

class Title final
	: public IScene {
public:
	Title();
	~Title();

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<EnemyManager> enemyManager_;
	Player* player_;

	std::unique_ptr<World> world_;
};