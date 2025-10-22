#include "ChaseState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

using namespace LWP;
using namespace LWP::Math;
using namespace FLMath;

ChaseState::ChaseState(BlackBoard* pBlackBoard, NodeResult* nodeResult, const float& speed) {
	pBlackBoard_ = pBlackBoard;
	nodeResult_ = nodeResult;
	speed_ = speed;
}

ChaseState::~ChaseState() {

}

void ChaseState::Init() {

}

void ChaseState::Update() {
	if (currentFrame_ <= 0.0f) {
		*nodeResult_ = (NodeResult::Success);
		isEnableChangeState_ = true;
	}

	// 自機アドレスを取得
	Actor* player = pBlackBoard_->GetValue<Actor*>("Player");
	// 敵アドレスを取得
	Actor* actor = pBlackBoard_->GetValue<Actor*>("Actor");

	// 方向ベクトル算出
	Vector3 vector = player->GetWorldTF()->GetWorldPosition() - actor->GetWorldTF()->GetWorldPosition();
	vector.y = 0.0f;

	// 速度を設定
	velocity_ = vector.Normalize() * speed_;
	// 速度に応じて角度変更
	if (Vector3::Dot(velocity_, velocity_) != 0.0f) {
		quat_ = LookRotationZLock(vector.Normalize());
	}
	else {
		quat_ = player->GetWorldTF()->rotation;
	}
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->SetRotation(quat_);

	currentFrame_--;
}

void ChaseState::DebugGui() {

}