#pragma once
#include "scene/IScene.h"
#include "../GameObjects/World/World.h"
#include "../GameObjects/BehaviourTree/BehaviorTreeUtility.h"

class Title final
	: public IScene {
public:
	Title() = default;
	~Title();

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

private: //*** これより先に必要な処理や変数を記述 ***//
	BehaviorTreeGraph* behaviorTree_;
	World* world_;
};