#include "CircleAttackLeaf.h"
#include "../Actor/BlackBoard.h"
#include "../Actor/Actor.h"
#include "../../../GameObjects/Enemy/State/AttackState.h"

CircleAttackLeaf::CircleAttackLeaf(BlackBoard* black_board)
	: LeafNodeBase(black_board)
{
}

CircleAttackLeaf::~CircleAttackLeaf() {

}

void CircleAttackLeaf::Tick() {
	// 攻撃状態に変更
	Actor* actor = blackBoard->GetValue<Actor*>("Actor");
	actor->ChangeState(new AttackState(blackBoard));
}

NodeResult CircleAttackLeaf::GetNodeResult() const {
	// 必ず成功を返す
	return NodeResult::Success;
}
