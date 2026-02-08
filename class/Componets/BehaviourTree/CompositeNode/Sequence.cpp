#include "Sequence.h"

Sequence::Sequence(BlackBoard* black_board)
	: CompositeNodeBase(black_board)
{

}

Sequence::~Sequence() {

}

void Sequence::Tick() {
	preRunningNodeIndex = runningNodeIndex;

	childNodes[runningNodeIndex]->Tick();
	auto result = childNodes[runningNodeIndex]->GetNodeResult();

	if (result == NodeResult::Success) {
		// 次回Sequenceに向けてノード番号を進める
		NodeIncrement();
		return;
	}

	// もし失敗が返されたらノード終了
	if (result == NodeResult::Fail) {
		Finalize();
	}

	nodeResult = result;
}

const int Sequence::GetNextIndex() const
{
	return runningNodeIndex + 1;
}

void Sequence::NodeIncrement()
{
	// 現在のノードの後始末
	childNodes[runningNodeIndex]->Finalize();

	// インデックスを進める
	runningNodeIndex = GetNextIndex();

	// もしすべての子ノードを回していたら
	if (runningNodeIndex > childNodes.size() - 1) {
		nodeResult = NodeResult::Success;
		Finalize();
		return;
	}

	// 次に回すノードの初期化
	childNodes[runningNodeIndex]->Init();
}
