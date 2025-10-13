#include "MeleeAttacker.h"
#include "../../World/IWorld.h"
#include "../../../Componets/BehaviourTree/Actor/BlackBoard.h"
#include "../../../Componets/BehaviourTree/BehaviourTreeBulider.h"
#include "../State/StateBase.h"

using namespace LWP::Math;

MeleeAttacker::MeleeAttacker(IWorld* world, LWP::Object::Camera* camera, std::string behavior_tree_file_path) {
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "Actor";
	attackPower_ = 1;

	model_.LoadSphere();
	model_.worldTF.translation = { 0,0,10 };

	blackBoard_ = new BlackBoard();
	blackBoard_->SetValue<Actor*>("Actor", this);

	// 自機のアドレスを設定
	auto* player = world->FindActor("Player");
	assert(player);
	blackBoard_->SetValue<Actor*>("Player", player);

	// ビヘイビアツリーの編集
	btEditor_ = std::make_unique<BehaviorTreeGraph>(true);
	btEditor_->SelectLoadFile("resources/json/Attacker_tree.json");

	// ビヘイビアツリー生成
	bt_ = BehaviourTreeBuilder::BuildAttackerTree("resources/json/Attacker_tree.json", blackBoard_);
	bt_->Init();
	bt_->Tick();
}

MeleeAttacker::~MeleeAttacker() {
	delete blackBoard_;
	delete bt_;
}

void MeleeAttacker::Init() {

}

void MeleeAttacker::Update() {
	// ビヘイビアツリー更新
	if (state_->GetIsEnableChangeState()) {	
		bt_->Tick();
	}

	// 状態
	state_->Update();

	// 移動処理
	Move();

	// 速度を初期化
	velocity_ = { 0.0f, 0.0f, 0.0f };
}

void MeleeAttacker::DrawGui() {
	btEditor_->SetRunnningNodeID(bt_->GetRunningNodeID());
	btEditor_->Update();
	btEditor_->Draw();
}

const int MeleeAttacker::GetBTRunningNodeID() const {
	return bt_->GetRunningNodeID();
}

void MeleeAttacker::Move() {
	if(state_) velocity_ = state_->GetVel();

	model_.worldTF.translation += velocity_;
	// 速度に応じて角度変更
	if (Vector3::Dot(velocity_, velocity_) != 0.0f) model_.worldTF.rotation = Quaternion::LookRotation(velocity_);
}