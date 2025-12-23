#include "ReadyToAttackState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/Math.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Math;
using namespace FLMath;

ReadyToAttackState::ReadyToAttackState(BlackBoard* pBlackBoard, NodeResult* nodeResult, const float& waitTime, const float& turnTime, const float& speed) {
	stopController_ = HitStopController::GetInstance();

	pBlackBoard_ = pBlackBoard;
	nodeResult_ = nodeResult;
	waitTime_ = waitTime;
	maxTurnTime_ = turnTime;
	turnTime_ = turnTime;
	speed_ = speed;

	// 方向ベクトル
	Vector3 dir = pBlackBoard_->GetValue<Actor*>("Player")->GetWorldTF()->GetWorldPosition() - pBlackBoard_->GetValue<Actor*>("Actor")->GetWorldTF()->GetWorldPosition();
	// 左右どちらに動くかをランダムで決める(0:左 1:右)
	int isFirstLRMove = Random::GenerateInt(0, 1);
	if (!isFirstLRMove) isFirstLRMove = -1;
	isFirstLRMove_ = static_cast<float>(isFirstLRMove);

	// 初速を決める
	velocity_ = isFirstLRMove_ * Vector3{ speed_,0,0 } *Matrix4x4::CreateRotateXYZMatrix(LookRotationZLock(dir).Normalize());
}

ReadyToAttackState::~ReadyToAttackState() {

}

void ReadyToAttackState::Init() {

}

void ReadyToAttackState::Update() {
	// うろつき終了
	if (waitTime_ <= 0.0f) {
		*nodeResult_ = (NodeResult::Success);
		isEnableChangeState_ = true;
		return;
	}

	// 移動
	Move();

	waitTime_ -= stopController_->GetDeltaTime();
	turnTime_ -= stopController_->GetDeltaTime();
}

void ReadyToAttackState::DebugGui() {

}

void ReadyToAttackState::Move() {
	// 今動いている方向と逆に動かす
	if (turnTime_ <= 0.0f) {
		isFirstLRMove_ *= -1.0f;
		turnTime_ = maxTurnTime_;
	}

	// 敵アドレスを取得
	Actor* actor = pBlackBoard_->GetValue<Actor*>("Actor");

	// 方向ベクトル
	Vector3 dir = pBlackBoard_->GetValue<Actor*>("Player")->GetWorldTF()->GetWorldPosition() - pBlackBoard_->GetValue<Actor*>("Actor")->GetWorldTF()->GetWorldPosition();

	// 体の向きを自機に向ける
	quat_ = LookRotationZLock(dir.Normalize());
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->SetRotation(quat_);

	// 速度を求める
	velocity_ = isFirstLRMove_ * Vector3{ speed_,0,0 } *Matrix4x4::CreateRotateXYZMatrix(quat_);
}