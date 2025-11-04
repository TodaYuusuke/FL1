#include "IGun.h"
#include "../../Bullets/BulletManager.h"
#include "../../Bullets/Bullet/Bullet.h"
#include "../../Collision/CollisionMask.h"

using namespace FLMath;
using namespace LWP::Math;

IGun::IGun(WeaponData data) {
	// ヒットストップ
	stopController_ = HitStopController::GetInstance();
	// 弾管理クラスのアドレス取得
	pBulletManager_ = BulletManager::GetInstance();

	// 調整項目を代入
	data_ = data;
	name_ = data_.modelName;

	// モデル生成
	body_.LoadFullPath("resources/model/Weapon/Gun/" + data_.modelName);
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン作成
	magazine_ = std::make_unique<Magazine>(data_.bulletNum);

	Init();
}

void IGun::Init() {
	body_.worldTF.translation = { 0.0f,0.0f,0.0f };
	body_.worldTF.rotation = { 0.0f,0.0f,0.0f,1.0f };
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン初期化
	magazine_->Init(data_.bulletNum);

	// 攻撃力
	currentAttackValue_ = data_.attackValue;

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
	// 弾がなくなれば強制リロード(クールタイム)
	if (magazine_->GetEmpty()) {
		isDestroy_ = true;
		isAttack_ = false;
		Reload();
	}

	// 攻撃指示
	AttackCommond();

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
		ImGui::TreePop();
	}
}

void IGun::Attack(int bulletHitFragBit) {
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
}

void IGun::Reload() {
	reloadFrame_ -= stopController_->GetDeltaTime();

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
		Vector3 randomVec{};
		// 単発で撃つならランダムにしない
		if (data_.sameBulletNum > 1) {
			Vector3 min = data_.diffusingBulletRange * -1.0f;
			Vector3 max = data_.diffusingBulletRange;
			randomVec = LWP::Utility::Random::GenerateVector3(min, max);
		}
		// 弾生成
		Bullet* bullet = new Bullet(body_.worldTF.GetWorldPosition(), randomVec + shotDirVel_ * 1.0f, bulletHitFragBit_);
		pBulletManager_->CreateBullet(bullet);

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
