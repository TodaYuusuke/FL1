#pragma once
#include "IMovement.h"
#include "../../../../Componets/BehaviourTree/Actor/Actor.h"
#include "Adapter.h"

class HomingMove : public IMovement {
public:
	// コンストラクタ
	HomingMove(Actor* target);
	// デストラクタ
	~HomingMove() override = default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="b"></param>
	void Update(BulletBase* bullet) override;

private:// 外部から受け取る変数
	//Actor* target_;

private:
};