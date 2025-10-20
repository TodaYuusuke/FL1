#include "EscapeState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

using namespace LWP;
using namespace LWP::Math;

EscapeState::EscapeState(BlackBoard* pBlackBoard, NodeResult* nodeResult, const float& speed) {
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

	// 方向ベクトル算出
	Vector3 vector = player->GetWorldTF()->GetWorldPosition() - actor->GetWorldTF()->GetWorldPosition();

	// 速度を設定
	velocity_ = vector.Normalize() * speed_;

	currentFrame_--;
}

void EscapeState::DebugGui() {

}