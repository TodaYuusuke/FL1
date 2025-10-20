#pragma once
#include "StateBase.h"

/// <summary>
/// 離脱状態
/// </summary>
class EscapeState : public StateBase {
public:
	// コンストラクタ
	EscapeState(BlackBoard* pBlackBoard, NodeResult* nodeResult, const float& speed);
	// デストラクタ
	~EscapeState() override;

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
	float speed_;

	float currentFrame_ = 60.0f;
};