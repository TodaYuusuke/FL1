#pragma once
#include "scene/IScene.h" 
#include "../GameObjects/World/World.h"
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/Enemy/EnemyManager.h"
#include "../GameObjects/Camera/FollowCamera/FollowCamera.h"

#include "../GameObjects/UI/ScoreUI/ScoreUI.h"
#include "../GameObjects/UI/BackGround.h"
#include "../GameObjects/UI/ScoreUI/RankingSystem.h"

#include "SceneChangeAnimation/SceneChangeAnimation.h"
#include "../Audio/AudioPlayer.h"

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

private: //ステート関数
	void Phase1KnockOut();
	void Phase2Score();
	void Phase3NextPage();
	void Phase4Ranking();
	void Phase5ToTitle();

private: // ** これより先に必要な処理や変数を記述 ** //

	// キー入力を促すスプライト
	LWP::Primitive::NormalSprite anyKeySprite_{};
	// ボタンのa値
	float keySpriteAlpha_ = 1.0f;
	// ボタンの点滅切り替えフラグ
	bool isSwitchFade_ = true;

	std::unique_ptr<ScoreUI> score_;
	std::unique_ptr<ScoreUI> knockOut_;

	//背景
	std::unique_ptr<BackGround> backGround_;

	//シーン遷移
	std::unique_ptr <DefaultSceneChangeAnimation> sceneChangeAnimation_;
	bool isChangeScene_ = false;

	int animationLength_ = 24;

	std::function<void()> state_;

	const std::string kJsonDirectoryPath = "Result/";
	LWP::Utility::JsonIO json_;

	LWP::Primitive::NormalSprite killCountSprite_;
	LWP::Object::TransformEuler killCountTransform_;

	LWP::Primitive::NormalSprite scoreSprite_;
	LWP::Object::TransformEuler scoreTransform_;

	int killCount_=0;
	int drawKillCount_=0;

	float scoreAnimationFrame_=0;
	float scoreAnimationLength_ = 0;

	std::unique_ptr<RankingSystem> rankingSystem_;
};