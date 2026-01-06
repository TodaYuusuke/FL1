#pragma once
#include "scene/IScene.h" 
#include "../GameObjects/World/World.h"
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/Enemy/EnemyManager.h"
#include "../GameObjects/Camera/FollowCamera/FollowCamera.h"
#include "../GameObjects/Tutorial/Tutorial.h"
#include "Adapter.h"

class TutorialScene final
	: public IScene {
public: // ** 純粋仮想関数の実体宣言 ** //
	TutorialScene();
	~TutorialScene();

	// 初期化
	void Initialize();
	// 更新
	void Update();

private:
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// 敵管理クラス
	std::unique_ptr<EnemyManager> enemyManager_;

	// ワールド
	std::unique_ptr<World> world_;

	// チュートリアル
	std::unique_ptr<Tutorial> tutorial_;
};