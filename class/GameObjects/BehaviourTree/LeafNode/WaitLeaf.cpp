#include "WaitLeaf.h"

WaitLeaf::WaitLeaf(BlackBoard* black_board, const float wait_time)
	: LeafNodeBase(black_board)
	, waitTime(wait_time)
	, waitCount(wait_time)
{
}

WaitLeaf::~WaitLeaf() {

}

void WaitLeaf::Tick()
{
	if (waitCount <= 0.f) {
		nodeResult = NodeResult::Success;
		return;
	}

	waitCount -= 1.f;
}

void WaitLeaf::Finalize()
{
	LeafNodeBase::Finalize();
	waitCount = waitTime;
}
