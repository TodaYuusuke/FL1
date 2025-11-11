#include "Cargo.h"
#include "../../World/IWorld.h"
#include "../../../Componets/BehaviourTree/Actor/BlackBoard.h"
#include "../../../Componets/BehaviourTree/BehaviourTreeBulider.h"
#include "../../Weapon/IWeapon.h"
#include "../State/StateBase.h"
#include "../../Collision/CollisionMask.h"

using namespace LWP::Math;
using namespace FLMath;

Cargo::Cargo(IWorld* world, int ID, const EnemyData& data) {
	stopController_ = HitStopController::GetInstance();

	world_ = world;
	// 製造番号
	ID_ = ID;
	// 調整情報
	data_ = data;
	// 識別タグ
	tag_ = EnemyConfig::tag;
	// 識別名
	name_ = EnemyConfig::name + std::to_string(ID_);
	// HP
	hp_ = std::make_unique<Health>(data_.hp);
	// 攻撃力
	attackPower_ = 1;

	// モデル生成
	model_.LoadFullPath(data.modelName);

	// 黒板生成
	blackBoard_ = new BlackBoard();
	blackBoard_->SetValue<Actor*>(EnemyConfig::name, this);
	// 自機のアドレスを設定
	auto* player = world->FindActor("Player");
	assert(player);
	blackBoard_->SetValue<Actor*>("Player", player);

	// ビヘイビアツリーの編集クラス
	btEditor_ = std::make_unique<BehaviorTreeGraph>(false);
	btEditor_->SelectLoadFile(data.BTFileName);
	// ビヘイビアツリー生成
	bt_ = BehaviourTreeBuilder::BuildAttackerTree(data.BTFileName, blackBoard_);
	bt_->Init();

	// 体の判定生成
	LWP::Math::Vector3 size = model_.worldTF.scale / 2.0f;
	bodyAABB_.min = size * -1.0f;
	bodyAABB_.max = size;
	bodyCollision_.name = name_;
	bodyCollision_.SetFollow(&model_.worldTF);
	bodyCollision_.isActive = true;
	// 自機の所属しているマスクを設定
	bodyCollision_.mask.SetBelongFrag(GameMask::enemy);
	// 当たり判定をとる対象のマスクを設定
	bodyCollision_.mask.SetHitFrag(GameMask::attack);
	bodyCollision_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		OnCollision(hitTarget);
		};
}

Cargo::~Cargo() {
	delete blackBoard_;
	delete bt_;
}

//void Cargo::DrawGui() {
//	if (ImGui::TreeNode("Cargo")) {
//		//btEditor_->SetRunnningNodeID(bt_->GetRunningNodeID());
//		//btEditor_->Update();
//		//btEditor_->Draw();
//		ImGui::TreePop();
//	}
//}

const int Cargo::GetBTRunningNodeID() const {
	return bt_->GetRunningNodeID();
}