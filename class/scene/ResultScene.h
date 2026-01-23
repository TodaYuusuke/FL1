#pragma once
#include "scene/IScene.h" 
#include "../GameObjects/World/World.h"
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/Enemy/EnemyManager.h"
#include "../GameObjects/Camera/FollowCamera/FollowCamera.h"

#include "../GameObjects/UI/ScoreUI/ScoreUI.h"
#include "../GameObjects/UI/BackGround.h"

#include "SceneChangeAnimation/SceneChangeAnimation.h"

#include "Adapter.h"

class ResultScene final
	: public IScene {
public: // ** 純粋仮想関数の実体宣言 ** //
	ResultScene();
	~ResultScene();

	// 初期化
	void Initialize();
	// 更新
	void Update();

	void ChangeTitleScene();

private: // ** これより先に必要な処理や変数を記述 ** //

	// キー入力を促すスプライト
	LWP::Primitive::NormalSprite anyKeySprite_{};

	std::unique_ptr<ScoreUI> score_;

	//背景
	std::unique_ptr<BackGround> backGround_;

	//シーン遷移
	std::unique_ptr <DefaultSceneChangeAnimation> sceneChangeAnimation_;
	bool isChangeScene_ = false;

	int animationLength_ = 24;
};