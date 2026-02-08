#include "Selector.h"

Selector::Selector(BlackBoard* black_board)
	: CompositeNodeBase(black_board)
{

}

Selector::~Selector() {

}

void Selector::Tick() {
	preRunningNodeIndex = runningNodeIndex;

	childNodes[runningNodeIndex]->Tick();
	auto result = childNodes[runningNodeIndex]->GetNodeResult();

	if (result == NodeResult::Fail) {
		// 次回Sequenceに向けてノード番号を進める
		NodeIncrement();
		return;
	}

	// もし成功が返されたらノード終了
	if (result == NodeResult::Success) {
		Finalize();
	}

	nodeResult = result;
}

const int Selector::GetNextIndex() const
{
	return runningNodeIndex + 1;
}

void Selector::NodeIncrement()
{
	// 現在のノードの後始末
	childNodes[runningNodeIndex]->Finalize();

	// インデックスを進める
	runningNodeIndex = GetNextIndex();

	// もしすべての子ノードを回していたら
	if (runningNodeIndex > childNodes.size() - 1) {
		nodeResult = NodeResult::Fail;
		Finalize();
		return;
	}

	// 次に回すノードの初期化
	childNodes[runningNodeIndex]->Init();
}
