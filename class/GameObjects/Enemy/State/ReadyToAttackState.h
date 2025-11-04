#pragma once
#include "StateBase.h"

/// <summary>
/// 待機状態
/// </summary>
class ReadyToAttackState : public StateBase {
public:
	// コンストラクタ
	ReadyToAttackState(BlackBoard* pBlackBoard, NodeResult* nodeResult, const float& waitTime);
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
	float waitTime_;
};