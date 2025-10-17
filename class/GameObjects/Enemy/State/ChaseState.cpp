#include "ChaseState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"

using namespace LWP;
using namespace LWP::Math;

ChaseState::ChaseState(BlackBoard* pBlackBoard, const float& speed) {
	pBlackBoard_ = pBlackBoard;
	speed_ = speed;
}

ChaseState::~ChaseState() {

}

void ChaseState::Init() {

}

void ChaseState::Update() {
	if (currentFrame_ <= 0.0f) {
		isEnableChangeState_ = true;
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

void ChaseState::DebugGui() {

}