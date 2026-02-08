#include "RandomSelector.h"
#include <Adapter.h>

using namespace LWP;
using namespace LWP::Utility;

RandomSelector::RandomSelector(BlackBoard* black_board)
	: CompositeNodeBase(black_board)
{

}

RandomSelector::~RandomSelector() {

}

void RandomSelector::Tick() {
	preRunningNodeIndex = runningNodeIndex;

	// 行動をランダムで決める
	runningNodeIndex = Random::GenerateInt(0, (int)childNodes.size() - 1);

	childNodes[runningNodeIndex]->Tick();
	auto result = childNodes[runningNodeIndex]->GetNodeResult();

	nodeResult = result;
}