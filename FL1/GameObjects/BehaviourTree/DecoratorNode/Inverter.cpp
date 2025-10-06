#include "Inverter.h"

#include <assert.h>

Inverter::Inverter(BlackBoard* black_board, INode* child_node)
	: DecoratorNodeBase(black_board)
{
	SetNode(child_node);
}

Inverter::~Inverter()
{
}

void Inverter::Tick()
{
	// 子ノードを実行
	childNode->Tick();
	// 子ノードの結果を取得
	NodeResult result = childNode->GetNodeResult();
	// 結果を反転させる
	if (result == NodeResult::Success) {
		nodeResult = NodeResult::Fail;
		return;
	}
	else if (result == NodeResult::Fail) {
		nodeResult = NodeResult::Success;
		return;
	}

	// 子ノードが実行中の場合は、Inverterも実行中にする
	nodeResult = NodeResult::Running;
}
