#include "AttackState.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../Weapon/IWeapon.h"

using namespace LWP;
using namespace LWP::Math;
using namespace FLMath;

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
	// 武器のアドレスを取得
	IWeapon* weapon = pBlackBoard_->GetValue<Actor*>("Actor")->GetWeapon();
	Vector3 weaponPos{};
	if (weapon) { weaponPos = weapon->GetWorldTF()->GetWorldPosition(); }

	// 敵と自機との方向ベクトル算出
	Vector3 vector = player->GetWorldTF()->GetWorldPosition() - actor->GetWorldTF()->GetWorldPosition();
	// 方向ベクトルに応じて角度変更
	quat_ = LookRotationZLock(Vector3{vector.x, 0.0f, vector.z }.Normalize());
	// 体の向きを変更
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->SetRotation(quat_);

	// 武器と自機との方向ベクトル算出
	Vector3 weaponVector = player->GetWorldTF()->GetWorldPosition() - weaponPos;
	// X軸の回転角算出[pi]
	float pitch = std::atan2(-weaponVector.y, std::sqrt(weaponVector.x * weaponVector.x + weaponVector.z * weaponVector.z));
	// 武器の角度変更
	if(weapon) weapon->SetRotation((Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, pitch)));

	// 持っている武器で攻撃
	pBlackBoard_->GetValue<Actor*>(EnemyConfig::name)->Attack();

	currentFrame_--;
}

void AttackState::DebugGui() {

}