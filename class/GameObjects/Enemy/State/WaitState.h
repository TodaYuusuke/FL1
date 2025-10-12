#pragma once
#include "StateBase.h"

/// <summary>
/// 待機状態
/// </summary>
class WaitState : public StateBase {
public:
	// コンストラクタ
	WaitState(BlackBoard* pBlackBoard, const float& waitTime);
	// デストラクタ
	~WaitState() override;

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