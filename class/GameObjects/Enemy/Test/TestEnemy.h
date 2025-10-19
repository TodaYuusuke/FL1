#pragma once
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include <Adapter.h>

/// <summary>
/// 動作確認用の敵
/// </summary>
class TestEnemy : public Actor {
public:
	//コンストラクタ
	TestEnemy();
	//デストラクタ
	~TestEnemy() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 調整項目
	/// </summary>
	void DrawGui() override;

};

