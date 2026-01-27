#include "Missile.h"
#include "../../../Attack/AttackManager.h"
#include "../../../Camera/Effect/CameraEffectHandler.h"

using namespace FLMath;
using namespace LWP::Math;

Missile::Missile(WeaponData gunData)
	: IGun(gunData)
{
	// 射撃エフェクト生成
	attackEffectEmitter_ = EffectManager::GetInstance()->CreateNewEmitter("MuzzleFlash", { 0.0f, 0.0f, 0.0f }, true);
	// 射撃エフェクトを武器モデルのマズルに親子付け
	attackEffectEmitter_->SetParent(&body_, "Muzzle");
	// 粒子の自動生成OFF
	attackEffectEmitter_->SetIsAutoEmit(false);
}

void Missile::Attack(int bulletHitFragBit, int bulletBelongFragBit, Actor* attackTarget) {
	// 弾がない状態なら撃てない
	if (magazine_->GetEmpty()) {
		isDestroy_ = true;
		return;
	}
	// 射撃できる状態か
	if (!GetIsEnableAttack()) { return; }
	// 射撃不可時間なら終了
	if (GetIsCoolTime()) { return; }

	// 追尾対象
	target_ = attackTarget;

	isAttack_ = true;
	bulletHitFragBit_ = bulletHitFragBit;
	bulletBelongFragBit_ = bulletBelongFragBit;
}

void Missile::AttackCommond() {
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
		pBulletManager_->CreateAttack(data_.bulletType, target_, body_.GetJointWorldPosition("Muzzle_0"), bulletHitFragBit_, bulletBelongFragBit_, randomVec.Normalize() * 1.0f, attackMultiply_);

		// 所持者が自機なら演出開始
		if (actor_->GetName() == "Player") {
			// カメラ揺れ
			CameraEffectHandler::GetInstance()->StartShake(Vector3{ 0.005f, 0.005f ,0.005f }, 0.1f);
			// ヒットストップ
			stopController_->Start(1.0f / 60.0f, 0.0f);
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

void Missile::FallingUpdate() {
	// 持ち主がいるなら処理しない
	if (actor_) { return; }
	// 地面に到達したら処理しない
	if (body_.GetJointWorldPosition("Muzzle_0").y <= 0.0f) { return; }

	// 重力
	velocity_.y -= 0.008f;
	// 前後左右の動きを徐々に遅くする
	velocity_.x *= 0.99f;
	velocity_.z *= 0.99f;

	body_.worldTF.translation += velocity_;
}

void Missile::SetWeaponSide(int weaponSide) {
	// 向きのある武器の名前を決める
	std::string side{};
	// 左側
	if (weaponSide == (int)WeaponSide::kLeft || weaponSide == (int)WeaponSide::kLeftShoulder) {
		side = "_L";
	}
	// 右側
	else if (weaponSide == (int)WeaponSide::kRight || weaponSide == (int)WeaponSide::kRightShoulder) {
		side = "_R";
	}

	// モデル名
	std::string modelName = data_.modelName;
	// 向き名を差し込む
	std::size_t pos = modelName.find('.');
	if (pos != std::string::npos && pos > 0) {
		modelName.insert(pos, side);
	}
	// モデル再読み込み
	body_.LoadFullPath(modelName);
}