#include "TimeLimit.h"
#include <Adapter.h>

TimeLimit::TimeLimit(BlackBoard* blackboard, INode* true_node, INode* false_node, float limit)
	: BranchNodeBase(blackboard, true_node, false_node)
{
	limit_ = limit;
}

TimeLimit::~TimeLimit() {
	BranchNodeBase::~BranchNodeBase();
}

void TimeLimit::Init() {
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

void TimeLimit::Tick() {
	branchNodes[satisfyIndex]->Tick();
	// 現在のノード状態を更新
	nodeResult = branchNodes[satisfyIndex]->GetNodeResult();


	if (nodeResult == NodeResult::Fail) {
		elapsed_ = 0.0f;
	}
}

const bool TimeLimit::IsCondition() {
	elapsed_ +=LWP::Info::GetDeltaTimeF() * 60.0f;

	// 時間を過ぎているならfalse
	if (elapsed_ >= limit_) {
		return false;
	}

	return true;
}