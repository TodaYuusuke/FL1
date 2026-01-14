#pragma once
#include "LeafNodeBase.h"

/// <summary>
/// 指定した時間待機する葉ノード
/// </summary>
class ReadyToAttackLeaf : public LeafNodeBase {
public:
	explicit ReadyToAttackLeaf(BlackBoard* blackBoard, const float& waitTime, const float& turnTime, const float& speed);
	~ReadyToAttackLeaf();

	void Tick() override;
	void Finalize() override;

private:
	// 待機時間
	float waitTime_ = 0.0f;
	// 経過時間
	float waitCount_ = 0.0f;
	// 折り返し時間
	float turnTime_ = 0.0f;
	// 移動速度
	float moveSpeed_ = 1.0f;
};