#include "AttackState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../Weapon/IWeapon.h"

using namespace LWP;
using namespace LWP::Math;

AttackState::AttackState(BlackBoard* pBlackBoard, NodeResult* nodeResult) {
	pBlackBoard_ = pBlackBoard;
	nodeResult_ = nodeResult;
}

AttackState::~AttackState() {

}

void AttackState::Init() {

}

void AttackState::Update() {
	if (currentFrame_ <= 0.0f) {
		*nodeResult_ = (NodeResult::Success);
		isEnableChangeState_ = true;
		return;
	}

	// 自機アドレスを取得
	Actor* player = pBlackBoard_->GetValue<Actor*>("Player");
	// 敵アドレスを取得
	Actor* actor = pBlackBoard_->GetValue<Actor*>("Actor");

	// 方向ベクトル算出
	Vector3 vector = player->GetWorldTF()->GetWorldPosition() - actor->GetWorldTF()->GetWorldPosition();
	// 方向ベクトルに応じて角度変更
	quat_ = Quaternion::LookRotation(vector.Normalize());

	// 体の向きを変更
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->SetRotation(quat_);

	// 持っている武器で攻撃
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->Attack();

	currentFrame_--;
}

void AttackState::DebugGui() {

}