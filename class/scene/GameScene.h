#pragma once
#include "scene/IScene.h" 
#include "../GameObjects/World/World.h"
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/Enemy/EnemyManager.h"
#include "../GameObjects/Camera/FollowCamera/FollowCamera.h"
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

private: // ** これより先に必要な処理や変数を記述 ** //
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// 敵管理クラス
	std::unique_ptr<EnemyManager> enemyManager_;

	// ワールド
	std::unique_ptr<World> world_;
};