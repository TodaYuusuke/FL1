#include "BranchNodeBase.h"

BranchNodeBase::BranchNodeBase(BlackBoard* black_board, INode* true_node, INode* false_node)
	: NodeBase(black_board)
{
	// [0]->trueノード
	// [1]->falseノード
	branchNodes[0] = true_node;
	branchNodes[1] = false_node;
}

BranchNodeBase::~BranchNodeBase()
{
	// ブランチノードの配列を解放
	for (int i = 0; i < 2; ++i) {
		if (branchNodes[i] != nullptr) {
			delete branchNodes[i];
			branchNodes[i] = nullptr;
		}
	}
}

void BranchNodeBase::Init()
{
	NodeBase::Init();

	// 条件を満たしたか
	if (IsCondition()) {
		// trueノード
		satisfyIndex = 0;
	}
	else {
		// falseノード
		satisfyIndex = 1;
	}

	// 該当のノードの初期化
	branchNodes[satisfyIndex]->Init();
}

void BranchNodeBase::Tick()
{
	branchNodes[satisfyIndex]->Tick();
	// 現在のノード状態を更新
	nodeResult = branchNodes[satisfyIndex]->GetNodeResult();
}

void BranchNodeBase::Finalize()
{
	NodeBase::Finalize();
	branchNodes[satisfyIndex]->Finalize();
	satisfyIndex = -1;
}

int BranchNodeBase::GetRunningNodeID() const
{
	return branchNodes[satisfyIndex]->GetRunningNodeID();
}
