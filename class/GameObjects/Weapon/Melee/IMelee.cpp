#include "IMelee.h"
#include "../../Bullets/BulletManager.h"
#include "../../Bullets/Melee/MeleeAttack.h"
#include "../../Collision/CollisionMask.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace FLMath;

IMelee::IMelee(WeaponData data) {
	// ヒットストップ
	stopController_ = HitStopController::GetInstance();
	// 弾管理クラスのアドレス取得
	pBulletManager_ = BulletManager::GetInstance();

	// 調整項目を代入
	data_ = data;
	name_ = data_.name;

	// モデル生成
	body_.LoadFullPath("resources/model/Weapon/Melee/" + data_.modelName);
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン作成
	magazine_ = std::make_unique<Magazine>(data_.bulletNum);

	Init();
}

IMelee::~IMelee() {
}

void IMelee::Init() {
	body_.worldTF.translation = { 0.0f,0.0f,0.0f };
	body_.worldTF.rotation = { 0.0f,0.0f,0.0f,1.0f };
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	assistPos_ = { 0.0f,0.0f,0.0f };

	// マガジン初期化
	magazine_->Init(data_.bulletNum);

	attackMultiply_ = 1.0f;
	speedMultiply_ = 1.0f;

	// 攻撃力
	currentAttackValue_ = data_.attackValue * attackMultiply_;

	// 射撃時の経過時間
	attackFrame_ = data_.shotIntervalTime * 60.0f;
	// リロードの経過時間
	reloadFrame_ = data_.coolTime * 60.0f;

	// 攻撃中か
	isAttack_ = false;
}

void IMelee::Update() {
	// 持ち主がいないときのみ落下
	FallingUpdate();

	// 攻撃力
	currentAttackValue_ = data_.attackValue * attackMultiply_;

	// 弾がなくなれば強制リロードor破壊(クールタイム)
	if (magazine_->GetEmpty()) {
		if (GetIsEnableAttack()) {
			isDestroy_ = true;
			Reload();
		}
	}
	// 攻撃可能状態
	if (GetIsEnableAttack()) {
		assistPos_ = { 0.0f,0.0f,0.0f };
		isAttack_ = false;
	}

	// 攻撃のアシスト
	AttackAssist();

	attackFrame_ -= stopController_->GetDeltaTime();

	attackFrame_ = std::max<float>(attackFrame_, 0.0f);
	reloadFrame_ = std::max<float>(reloadFrame_, 0.0f);
}

void IMelee::DebugGui() {
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
				ImGui::DragFloat("Speed", &data_.bulletSpeed);
				ImGui::TreePop();
			}
			// 溜め時間
			ImGui::DragFloat("Store", &data_.storeTime);
			// 攻撃力
			ImGui::DragFloat("AttackPower", &data_.attackValue);
			// 撃てない時間
			ImGui::DragFloat("CoolTime", &data_.coolTime);
			// レアリティ
			ImGui::DragInt("Rarity", &data_.rarity);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Energy")) {
			magazine_->DebugGui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("CoolTime")) {
			ImGui::DragFloat("AttackFrame", &attackFrame_);
			ImGui::DragFloat("ReloadFrame", &coolFrame_);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void IMelee::Attack(int bulletHitFragBit, Actor* attackTarget) {
	// 弾がない状態なら撃てない
	if (magazine_->GetEmpty()) {
		isDestroy_ = true;
		return;
	}
	// 射撃できる状態か
	if (!GetIsEnableAttack()) { return; }

	// 攻撃判定生成
	MeleeAttack* bullet = new MeleeAttack(data_.bulletSize, &body_.worldTF, bulletHitFragBit, data_.attackValue, data_.bulletElapsedTime);
	pBulletManager_->CreateBullet(bullet);

	// 弾数を減らす
	magazine_->BulletDecrement();
	// 射撃間隔を初期化
	attackFrame_ = data_.shotIntervalTime * 60.0f;
	// 攻撃中
	isAttack_ = true;

	assistPos_ = actor_->GetWorldTF()->GetWorldPosition();
	// 攻撃対象あり
	if (attackTarget) {
		// 攻撃対象との距離
		Vector3 dist = attackTarget->GetWorldTF()->GetWorldPosition() - actor_->GetWorldTF()->GetWorldPosition();
		float length = dist.Length();
		if (length >= assistOffset) length -= assistOffset;
		else length = 0.0f;

		if (length >= 20.0f) {
			// アシスト後の座標
			assistPos_ += Vector3{ 0.0f,0.0f,2.0f } *LWP::Math::Matrix4x4::CreateRotateXYZMatrix(actor_->GetWorldTF()->rotation);
		}
		else {
			// アシスト後の座標
			assistPos_ += Vector3{ 0.0f,0.0f,length } *LWP::Math::Matrix4x4::CreateRotateXYZMatrix(LWP::Math::Quaternion::ConvertDirection(dist));
		}
	}
	// 攻撃対象なし
	else {
		// アシスト後の座標
		assistPos_ += Vector3{ 0.0f,0.0f,2.0f } * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(actor_->GetWorldTF()->rotation);
	}
}

void IMelee::Reload() {
	reloadFrame_ -= stopController_->GetDeltaTime();
	isAttack_ = false;
	isDestroy_ = false;

	// リロード完了
	if (!GetIsReloadTime()) {
		// リロード時間を初期化
		reloadFrame_ = data_.coolTime * 60.0f;
		// 弾数を初期化
		magazine_->Init(data_.bulletNum);
	}
}

void IMelee::Destroy() {

}

void IMelee::FallingUpdate() {
	// 持ち主がいるなら処理しない
	if (actor_) { return; }
	// 地面に到達したら処理しない
	if (body_.worldTF.GetWorldPosition().y <= body_.worldTF.scale.y) { return; }

	// 重力
	velocity_.y -= 0.008f;
	// 前後左右の動きを徐々に遅くする
	velocity_.x *= 0.99f;
	velocity_.z *= 0.99f;

	body_.worldTF.translation += velocity_;
}

void IMelee::AttackAssist() {
	if (!GetIsAttack()) { return; }
	if (!actor_) { return; }

	// 速度
	Vector3 vel{};
	Vector3 radian{};
	Quaternion quat = { 0.0f,0.0f,0.0f,1.0f };

	// 攻撃対象との距離
	Vector3 dist = assistPos_ - actor_->GetWorldTF()->GetWorldPosition();

	// 徐々に速度を上げる
	float frame = (data_.shotIntervalTime * 60.0f - attackFrame_) / (data_.shotIntervalTime * 60.0f);
	// 座標をイージング
	vel = Interpolation::Lerp(actor_->GetWorldTF()->GetWorldPosition(), assistPos_, frame);
	// 所持者の前回の座標と求めた座標の差分を求める
	vel -= actor_->GetWorldTF()->GetWorldPosition();
	vel.y = 0.0f;

	// 移動速度からラジアンを求める
	radian.y = GetRadian(Vector3{ 0.0f,0.0f,1.0f }, dist.Normalize(), Vector3{ 0.0f,1.0f,0.0f });
	quat = Quaternion::CreateFromAxisAngle(Vector3{ 0.0f,1.0f,0.0f }, radian.y);

	// 所持者の速度
	actor_->SetWeaponVelocity(vel);
}