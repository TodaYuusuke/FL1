#include "RunOnce.h"
#include <assert.h>

RunOnce::RunOnce(BlackBoard* black_board, INode* child_node)
	: DecoratorNodeBase(black_board)
{
	SetNode(child_node);
}

RunOnce::~RunOnce() {

}

void RunOnce::Tick() {
	// 子ノードを実行
	if (!hasRun_) childNode->Tick();
	// 子ノードの結果を取得
	NodeResult result = childNode->GetNodeResult();
	// 子ノードが成功したらフラグをtrue
    if (result == NodeResult::Success) {
        hasRun_ = true;
    }

	// 一度成功しているなら常にSuccessを返す
    if (hasRun_) nodeResult = NodeResult::Success;
}
