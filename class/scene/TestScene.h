#pragma once
#include "scene/IScene.h"
#include "../Effect/EffectManager.h"
#include "../Effect/EffectEditor.h"
#include "../Audio/SEPlayer.h"
#include "../Componets/Animation/AnimationManager.h"

class TestScene final
	: public IScene {
public:
	TestScene();
	~TestScene();

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

private: //*** これより先に必要な処理や変数を記述 ***//

	// テスト用モデル
	LWP::Resource::RigidModel testModel_;

	// ロボットモデル
	LWP::Resource::SkinningModel robotModel_{};
	// ロボットアニメーション
	LWP::Resource::Animation animation_{};
	// アニメーション管理クラス
	AnimationManager* animationManager_ = nullptr;


	float masterVolume_ = 1.0f;
};