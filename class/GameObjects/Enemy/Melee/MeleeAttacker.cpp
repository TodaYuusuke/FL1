#include "MeleeAttacker.h"
#include "../../World/IWorld.h"
#include "../../../Componets/BehaviourTree/Actor/BlackBoard.h"
#include "../../../Componets/BehaviourTree/BehaviourTreeBulider.h"
#include "../State/StateBase.h"
#include "../../Collision/CollisionMask.h"

using namespace LWP::Math;

MeleeAttacker::MeleeAttacker(IWorld* world, int ID, const EnemyData& data) {
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
	model_.Update();
	model_.materials["EmissionMaterial"].enableLighting = false;
	for (auto key : model_.skeleton.jointMap) {
		if (!key.first.empty()) jointName_.push_back(key.first);
	}

	// 黒板生成
	blackBoard_ = new BlackBoard();
	blackBoard_->SetValue<Actor*>(EnemyConfig::name, this);
	// 自機のアドレスを設定
	auto* player = world->FindActor("Player");
	assert(player);
	blackBoard_->SetValue<Actor*>("Player", player);

	// ビヘイビアツリーの編集クラス
	btEditor_ = std::make_unique<BehaviorTreeGraph>(true);
	btEditor_->SelectLoadFile(data.BTFileName);
	// ビヘイビアツリー生成
	bt_ = BehaviourTreeBuilder::BuildAttackerTree(data.BTFileName, blackBoard_);
	bt_->Init();

	// 体の判定生成
	bodyAABB_.min = data_.colliderMin;
	bodyAABB_.max = data_.colliderMax;
	bodyCollision_.name = name_;
	bodyCollision_.SetFollow(&model_.worldTF);
	bodyCollision_.isActive = true;
	// 所属しているマスクを設定
	bodyCollision_.mask.SetBelongFrag(GameMask::enemy);
	// 当たり判定をとる対象のマスクを設定
	bodyCollision_.mask.SetHitFrag(GameMask::attack);
	bodyCollision_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		OnCollision(hitTarget);
		};

	// アニメーションマネージャー作成
	animManager_ = std::make_unique<RobotAnimManager>("resources/model/Player/Player.gltf", &model_, GetVelocity(), &model_.worldTF.rotation);
	animManager_->Init();

	// 各アニメーション再生
	animManager_->PlayQue("Other_Idle", RobotAnimManager::PlayType::Other, 0.0f, true);
	animManager_->PlayQue("Idle", RobotAnimManager::PlayType::LeftArm, 0.0f, true);
	animManager_->PlayQue("Idle", RobotAnimManager::PlayType::RightArm, 0.0f, true);
	animManager_->PlayQue("Idle", RobotAnimManager::PlayType::LeftShoulder, 0.0f, true);
	animManager_->PlayQue("Idle", RobotAnimManager::PlayType::RightShoulder, 0.0f, true);
}

MeleeAttacker::~MeleeAttacker() {
	delete blackBoard_;
	delete bt_;
}

void MeleeAttacker::Init() {

}

void MeleeAttacker::DrawGui() {
	if (ImGui::TreeNode("MeleeAttacker")) {
		//btEditor_->SetRunnningNodeID(bt_->GetRunningNodeID());
		//btEditor_->Update();
		//btEditor_->Draw();
		ImGui::TreePop();
	}
}

const int MeleeAttacker::GetBTRunningNodeID() const {
	return bt_->GetRunningNodeID();
}

void MeleeAttacker::Move() {
	if (state_) {
		velocity_ = state_->GetVel();
		quat_ = state_->GetRot();
	}

	model_.worldTF.translation += velocity_;
}

void MeleeAttacker::AnimManagerUpdate()
{
	animManager_->Update();
}

void MeleeAttacker::PlayAttackAnim(const int weaponSide)
{
	// 武器の情報取得
	WeaponData data = weapons_[weaponSide]->GetWeaponData();

	// アニメーション名と効果音名の取得
	std::string animName = data.animName;
	// アニメーション名がない場合
	if (animName == "") { return; }

	// 武器タイプがミサイルか近接武器の場合アニメーション名変更
	// ミサイルか近接武器の場合待機アニメーションを変更する
	if (data.type == static_cast<int>(WeaponType::kMissile)) {
		animName = "Tilt" + animName;
	}

	std::string seName = data.attackSEFileName;

	// 射撃アニメーション再生
	animManager_->PlayDirect(animName, weaponSide + 2)
		.AddEvent("PlaySE", 1, [this, weaponSide, seName]() { SEPlayer::GetInstance()->PlaySE(seName, 1.0f, LWP::AudioConfig::Player); });

	// ミサイルか近接武器の場合待機アニメーションを変更する
	if (data.type == static_cast<int>(WeaponType::kMissile) || data.type == static_cast<int>(WeaponType::kMelee)) {
		// 待機アニメーションをキューに入れる
		animManager_->PlayQue("TiltIdle", weaponSide + 2);
	}
	else {
		// 待機アニメーションをキューに入れる
		animManager_->PlayQue("Idle", weaponSide + 2);
	}
}
