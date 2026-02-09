#pragma once
#include "scene/IScene.h" 
#include "../GameObjects/World/World.h"
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/Enemy/EnemyManager.h"
#include "../GameObjects/Camera/FollowCamera/FollowCamera.h"
#include "../GameObjects/Tutorial/Tutorial.h"
#include "../Componets/EasingEditor.h"
#include "SceneChangeAnimation/SceneChangeAnimationForPlane.h"
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


	/// <summary>
	/// ゲームシーンに遷移する関数
	/// </summary>
	void ChangeGameScene();

private:
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// 敵管理クラス
	std::unique_ptr<EnemyManager> enemyManager_;

	// プレイヤー
	Player* player_ = nullptr;

	// ワールド
	std::unique_ptr<World> world_;

	// チュートリアル
	std::unique_ptr<Tutorial> tutorial_;

	//シーン遷移
	std::unique_ptr <SceneChangeAnimationPlane> sceneChangeAnimation_;
	bool isChangeScene_ = false;
	int animationLength_ = 36;
	bool isEndStartAnimation_{};

	std::unique_ptr<EasingEditor> easingEditor_;
};