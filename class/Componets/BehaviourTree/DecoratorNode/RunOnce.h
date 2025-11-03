#pragma once
#include "DecoratorNodeBase.h"

class RunOnce : public DecoratorNodeBase {
public:
	// コンストラクタ
	explicit RunOnce(BlackBoard* black_board, INode* child_node);
	// デストラクタ
	~RunOnce();

	/// <summary>
	/// 更新
	/// </summary>
	void Tick() override;

private:
	bool hasRun_;
};