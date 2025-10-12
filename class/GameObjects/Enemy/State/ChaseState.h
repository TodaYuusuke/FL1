#pragma once
#include "StateBase.h"

/// <summary>
/// 追従状態
/// </summary>
class ChaseState : public StateBase {
public:
	// コンストラクタ
	ChaseState(BlackBoard* pBlackBoard, const float& speed);
	// デストラクタ
	~ChaseState() override;

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