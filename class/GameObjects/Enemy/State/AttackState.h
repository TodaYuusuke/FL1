#pragma once
#include "StateBase.h"

/// <summary>
/// 攻撃状態
/// </summary>
class AttackState : public StateBase {
public:
	// コンストラクタ
	AttackState(BlackBoard* pBlackBoard, NodeResult* nodeResult);
	// デストラクタ
	~AttackState() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	///	更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui() override;

private:
	LWP::Resource::RigidModel attackModel_;

	float currentFrame_ = 30.0f;
};