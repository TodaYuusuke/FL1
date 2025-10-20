#include "EscapeFromPlayerLeaf.h"
#include "../Actor/BlackBoard.h"
#include "../Actor/Actor.h"
#include "../../../GameObjects/Enemy/State/EscapeState.h"

using namespace LWP::Math;

EscapeFromPlayerLeaf::EscapeFromPlayerLeaf(BlackBoard* black_board, const float& speed)
	: LeafNodeBase(black_board)
{
	this->speed = speed;
}

EscapeFromPlayerLeaf::~EscapeFromPlayerLeaf() {

}

void EscapeFromPlayerLeaf::Tick() {
	// 敵アドレスを取得
	Actor* actor = blackBoard->GetValue<Actor*>("Actor");
	actor->ChangeState(new EscapeState(blackBoard, &nodeResult, speed));
}

NodeResult EscapeFromPlayerLeaf::GetNodeResult() const {
	// 必ず成功を返す
	return NodeResult::Success;
}
