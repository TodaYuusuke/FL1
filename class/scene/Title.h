#pragma once
#include "scene/IScene.h"
#include "../GameObjects/UI/BackGround.h"
#include "SceneChangeAnimation/SceneChangeAnimation.h"
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

	// タイトルロゴスプライト
	LWP::Primitive::NormalSprite titleLogoSprite_{};

	// キー入力を促すスプライト
	LWP::Primitive::NormalSprite anyKeySprite_{};

	//背景
	std::unique_ptr<BackGround> backGround_;

	//シーン遷移
	std::unique_ptr <DefaultSceneChangeAnimation> sceneChangeAnimation_;

	int animationLength_=24;
};