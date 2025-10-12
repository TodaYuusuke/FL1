#include "CompositeNodeBase.h"

CompositeNodeBase::~CompositeNodeBase() {
	for (auto node : childNodes) {
		delete node;
	}
	childNodes.clear();
}

void CompositeNodeBase::Init() {
	NodeBase::Init();
	runningNodeIndex = 0;

	// 最初のノードを初期化
	if (childNodes.size() > 0) {
		childNodes[runningNodeIndex]->Init();
	}
	else {
		nodeResult = NodeResult::Fail;
	}
}

void CompositeNodeBase::Finalize()
{
	NodeBase::Finalize();

	// 最初のノードを初期化
	runningNodeIndex = 0;
	childNodes[runningNodeIndex]->Init();
}

void CompositeNodeBase::AddNode(INode* node)
{
	childNodes.push_back(node);
}

int CompositeNodeBase::GetRunningNodeID() const
{
	// 現在動かしているノードのIDを返す
	return childNodes[runningNodeIndex]->GetRunningNodeID();
}
