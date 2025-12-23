#pragma once
#include "StateBase.h"

/// <summary>
/// 攻撃のタイミングをうかがっている状態(左右にうろつく)
/// </summary>
class ReadyToAttackState : public StateBase {
public:
	// コンストラクタ
	ReadyToAttackState(BlackBoard* pBlackBoard, NodeResult* nodeResult, const float& waitTime, const float& turnTime, const float& speed);
	// デストラクタ
	~ReadyToAttackState() override;

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
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

private:
	// 待機時間
	float waitTime_;
	// 左右にうろつく時間間隔
	float turnTime_;
	float maxTurnTime_;
	// 移動速度
	float speed_;

	float isFirstLRMove_;
};