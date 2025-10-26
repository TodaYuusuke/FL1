#include "Drone.h"
#include "../../World/IWorld.h"
#include "../../../Componets/BehaviourTree/Actor/BlackBoard.h"
#include "../../../Componets/BehaviourTree/BehaviourTreeBulider.h"
#include "../../Weapon/IWeapon.h"
#include "../State/StateBase.h"
#include "../EnemyConfig.h"
#include "../../Collision/CollisionMask.h"

using namespace LWP::Math;
using namespace FLMath;

Drone::Drone(IWorld* world, int ID, const std::string& BTFilePath) {
	world_ = world;
	ID_ = ID;
	tag_ = EnemyConfig::tag;
	name_ = EnemyConfig::name + std::to_string(ID_);
	attackPower_ = 1;

	// モデル生成
	model_.LoadSphere();

	// 黒板生成
	blackBoard_ = new BlackBoard();
	blackBoard_->SetValue<Actor*>(EnemyConfig::name, this);

	// 自機のアドレスを設定
	auto* player = world->FindActor("Player");
	assert(player);
	blackBoard_->SetValue<Actor*>("Player", player);

	// ビヘイビアツリーの編集クラス
	btEditor_ = std::make_unique<BehaviorTreeGraph>(false);
	btEditor_->SelectLoadFile(BTFilePath);

	// ビヘイビアツリー生成
	bt_ = BehaviourTreeBuilder::BuildAttackerTree(BTFilePath, blackBoard_);
	bt_->Init();

	// 体の判定生成
	bodyCollision_.SetFollow(&model_.worldTF);
	bodyCollision_.isActive = true;
	bodyCollision_.worldTF.translation = { 0.0f, 1.0f, 0.0f };
	// 自機の所属しているマスクを設定
	bodyCollision_.mask.SetBelongFrag(GameMask::enemy);
	// 当たり判定をとる対象のマスクを設定
	bodyCollision_.mask.SetHitFrag(GameMask::attack);
	bodyCollision_.enterLambda = [this](LWP::Object::Collision* hitTarget) {
		hitTarget;
		};
}

Drone::~Drone() {
	delete blackBoard_;
	delete bt_;
}

void Drone::Init() {

}

void Drone::Update() {	
	// 基底クラス
	Actor::Update();

	// 移動処理
	Move();

	// 速度を初期化
	velocity_ = { 0.0f, 0.0f, 0.0f };
}

void Drone::DrawGui() {
	if (ImGui::TreeNode("Drone")) {
		btEditor_->SetRunnningNodeID(bt_->GetRunningNodeID());
		btEditor_->Update();
		btEditor_->Draw();
		ImGui::TreePop();
	}
}

const int Drone::GetBTRunningNodeID() const {
	return bt_->GetRunningNodeID();
}

void Drone::Move() {
	if (state_) {
		velocity_ = state_->GetVel();
		quat_ = state_->GetRot();
	}

	model_.worldTF.translation += velocity_;
}