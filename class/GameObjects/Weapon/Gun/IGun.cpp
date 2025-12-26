#include "IGun.h"
#include "../../Attack/AttackManager.h"
#include "../../Collision/CollisionMask.h"
#include "../../Camera/Effect/CameraEffectHandler.h"

using namespace FLMath;
using namespace LWP::Math;

IGun::IGun(WeaponData data) {
	// ヒットストップ
	stopController_ = HitStopController::GetInstance();
	// 弾管理クラスのアドレス取得
	pBulletManager_ = AttackManager::GetInstance();

	// 調整項目を代入
	data_ = data;
	name_ = data_.name;

	// モデル生成
	body_.LoadFullPath(data_.modelName);
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン作成
	magazine_ = std::make_unique<Magazine>(data_.bulletNum);

	// 光柱
	lightPillar_.Init();
	lightPillar_.isActive = true;
	lightPillar_.LoadTexture("Weapon/pillar_triangle.png");
	unsigned int color = WeaponConfig::TextureName::LightPillar::Color::Weapon::color[data_.type];
	lightPillar_.material.color = LWP::Utility::Color(color);
	lightPillar_.material.color.A = 100;
	lightPillar_.anchorPoint = { 0.5f, 0.5f };
	lightPillar_.worldTF.scale = { 1.0f, 50.0f, 1.0f };
	lightPillar_.worldTF.translation = body_.worldTF.translation;

	Init();
}

void IGun::Init() {
	body_.worldTF.translation = { 0.0f,0.0f,0.0f };
	body_.worldTF.rotation = { 0.0f,0.0f,0.0f,1.0f };
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン初期化
	magazine_->Init(data_.bulletNum);

	attackMultiply_ = 1.0f;
	speedMultiply_ = 1.0f;

	// 攻撃力
	//currentAttackValue_ = data_.attackMultiply * attackMultiply_;

	// 射撃時の経過時間
	if (data_.burstNum > 0) {
		BurstMode();
		burstNum_ = data_.burstNum;
	}
	else {
		FullAutoMode();
	}

	// リロードの経過時間
	reloadFrame_ = data_.reloadTime * 60.0f;
	// 銃を撃つまでの時間
	storeFrame_ = data_.storeTime * 60.0f;
}

void IGun::Update() {
	// 持ち主がいないときのみ落下
	FallingUpdate();

	// 対象が死んでいる
	if (target_ && !target_->GetIsAlive()) {
		target_ = nullptr;
	}

	// 弾がなくなれば強制リロード(クールタイム)
	if (magazine_->GetEmpty()) {
		isDestroy_ = true;
		isAttack_ = false;
		Reload();
	}

	// 攻撃指示
	AttackCommond();

	if (actor_) lightPillar_.isActive = false;
	else lightPillar_.isActive = true;
	lightPillar_.worldTF.translation = body_.worldTF.GetWorldPosition();

	attackFrame_ -= stopController_->GetDeltaTime();
	coolFrame_ -= stopController_->GetDeltaTime();

	attackFrame_ = std::max<float>(attackFrame_, 0.0f);
	coolFrame_ = std::max<float>(coolFrame_, 0.0f);
	reloadFrame_ = std::max<float>(reloadFrame_, 0.0f);
	storeFrame_ = std::clamp<float>(storeFrame_, 0.0f, data_.storeTime * 60.0f);
}

void IGun::DebugGui() {
	// 現在の情報
	if (ImGui::TreeNode("CurrentData")) {
		if (ImGui::TreeNode("WeaponData")) {
			// 発射間隔
			if (ImGui::TreeNode("Interval")) {
				ImGui::DragFloat("Normal", &data_.shotIntervalTime);
				ImGui::DragFloat("Burst", &data_.burstIntervalTime);
				ImGui::TreePop();
			}
			// 弾
			if (ImGui::TreeNode("Bullet")) {
				ImGui::DragInt("Num", &data_.bulletNum);
				ImGui::TreePop();
			}
			// 溜め時間
			ImGui::DragFloat("Store", &data_.storeTime);
			// 撃てない時間
			ImGui::DragFloat("CoolTime", &data_.coolTime);
			// レアリティ
			ImGui::DragInt("Rarity", &data_.rarity);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Magazine")) {
			magazine_->DebugGui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("CoolTime")) {
			ImGui::DragFloat("ShotFrame", &attackFrame_);
			ImGui::DragFloat("ReloadFrame", &reloadFrame_);
			ImGui::DragFloat("CoolFrame", &coolFrame_);
			ImGui::TreePop();
		}

		ImGui::DragFloat("AttackMultiply", &attackMultiply_);

		ImGui::TreePop();
	}
}

void IGun::Attack(int bulletHitFragBit, int bulletBelongFragBit, Actor* attackTarget) {
	attackTarget;

	// 弾がない状態なら撃てない
	if (magazine_->GetEmpty()) {
		isDestroy_ = true;
		return;
	}
	// 射撃できる状態か
	if (!GetIsEnableAttack()) { return; }
	// 射撃不可時間なら終了
	if (GetIsCoolTime()) { return; }

	isAttack_ = true;
	bulletHitFragBit_ = bulletHitFragBit;
	bulletBelongFragBit_ = bulletBelongFragBit;
}

void IGun::Reload() {
	reloadFrame_ -= stopController_->GetDeltaTime();
	isDestroy_ = false;

	// リロード完了
	if (!GetIsReloadTime()) {
		// リロード時間を初期化
		reloadFrame_ = data_.coolTime * 60.0f;
		// 弾数を初期化
		magazine_->Init(data_.bulletNum);
	}
}

void IGun::Destroy() {

}

void IGun::FallingUpdate() {
	// 持ち主がいるなら処理しない
	if (actor_) { return; }
	// 地面に到達したら処理しない
	if (body_.GetJointWorldPosition("Muzzle").y <= 0.0f) { return; }

	// 重力
	velocity_.y -= 0.008f;
	// 前後左右の動きを徐々に遅くする
	velocity_.x *= 0.99f;
	velocity_.z *= 0.99f;

	body_.worldTF.translation += velocity_;
}

void IGun::AttackCommond() {
	// 射撃できる状態か
	if (!GetIsEnableAttack()) { return; }
	// 攻撃指示がない
	if (!isAttack_) {
		// 溜め時間をもとに戻していく
		storeFrame_ += stopController_->GetDeltaTime();
		return;
	}
	storeFrame_ -= stopController_->GetDeltaTime();
	// 溜め時間中なら撃てない
	if (GetIsStoreTime()) { return; }
	// 弾がない状態なら撃てない
	if (magazine_->GetEmpty()) { return; }
	// 射撃不可時間なら終了
	if (GetIsCoolTime()) { return; }

	// 弾を撃つ
	int i = data_.sameBulletNum;
	do {
		Vector3 randomVec = shotDirVel_;
		// 単発で撃つならランダムにしない
		if (data_.sameBulletNum > 1) {
			Vector3 min = data_.diffusingBulletRange * -1.0f;
			Vector3 max = data_.diffusingBulletRange;
			randomVec = LWP::Utility::Random::GenerateVector3(min, max);
			randomVec.z = 1.0f;

			randomVec = Vector3{ 0,0,1 } *
				(Matrix4x4::DirectionToDirection(Vector3{ 0,0,1 }, randomVec.Normalize()) * Matrix4x4::DirectionToDirection(Vector3{ 0,0,1 }, shotDirVel_));
		}
		// 弾生成
		pBulletManager_->CreateAttack(data_.bulletType, target_, body_.GetJointWorldPosition("Muzzle"), bulletHitFragBit_, bulletBelongFragBit_, randomVec.Normalize() * 1.0f, attackMultiply_);
		
		// 所持者が自機ならカメラ演出
		if (actor_->GetName() == "Player") {
			CameraEffectHandler::GetInstance()->StartShake(Vector3{ 0.005f, 0.005f ,0.005f }, 0.1f);
		}

		i--;
	} while (i > 0);

	// 弾数を減らす
	magazine_->BulletDecrement();

	// 射撃間隔を初期化
	if (shotType_ == ShotType::kBurst) {
		BurstMode();
		burstNum_--;
		// バースト数を超えたら撃てなくする
		if (burstNum_ <= 0) {
			// 射撃不可時間開始
			coolFrame_ = data_.coolTime * 60.0f;
			burstNum_ = data_.burstNum;
			isAttack_ = false;
		}
	}
	else {
		FullAutoMode();
		isAttack_ = false;
	}
}

void IGun::BurstMode() {
	shotType_ = ShotType::kBurst;
	attackFrame_ = data_.burstIntervalTime * 60.0f;
}

void IGun::FullAutoMode() {
	shotType_ = ShotType::kFullAuto;
	attackFrame_ = data_.shotIntervalTime * 60.0f;
}