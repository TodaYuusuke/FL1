#include "EscapeState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

using namespace LWP;
using namespace LWP::Math;
using namespace FLMath;

EscapeState::EscapeState(BlackBoard* pBlackBoard, NodeResult* nodeResult, const float& speed) {
	stopController_ = HitStopController::GetInstance();

	pBlackBoard_ = pBlackBoard;
	nodeResult_ = nodeResult;
	speed_ = speed;
}

EscapeState::~EscapeState() {

}

void EscapeState::Init() {

}

void EscapeState::Update() {
	if (currentFrame_ <= 0.0f) {
		*nodeResult_ = (NodeResult::Success);
		isEnableChangeState_ = true;
		return;
	}

	// 自機アドレスを取得
	Actor* player = pBlackBoard_->GetValue<Actor*>("Player");
	// 敵アドレスを取得
	Actor* actor = pBlackBoard_->GetValue<Actor*>("Actor");

	// 方向ベクトル算出して反転
	Vector3 vector = (player->GetWorldTF()->GetWorldPosition() - actor->GetWorldTF()->GetWorldPosition()) * -1.0f;
	vector.y = 0.0f;

	// 速度を設定
	velocity_ = vector.Normalize() * speed_;
	// 速度に応じて角度変更
	if (Vector3::Dot(velocity_, velocity_) != 0.0f) {
		// 自機の方を常に向きたいのでベクトルを反転して自機に身体を向ける
		quat_ = LookRotationZLock(vector.Normalize() * -1.0f);
	}
	else {
		quat_ = player->GetWorldTF()->rotation;
	}
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->SetRotation(quat_);

	currentFrame_ -= stopController_->GetDeltaTime();
}

void EscapeState::DebugGui() {

}