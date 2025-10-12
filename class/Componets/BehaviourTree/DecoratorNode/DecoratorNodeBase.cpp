#include "DecoratorNodeBase.h"

DecoratorNodeBase::~DecoratorNodeBase()
{
	if (childNode) {
		delete childNode;
		childNode = nullptr;
	}
}

void DecoratorNodeBase::Init()
{
	NodeBase::Init();
	childNode->Init();
}

void DecoratorNodeBase::Finalize()
{
	NodeBase::Finalize();
	childNode->Finalize();
}

void DecoratorNodeBase::SetNode(INode* node)
{
	childNode = node;
}

int DecoratorNodeBase::GetRunningNodeID() const
{
	return childNode->GetRunningNodeID();
}
