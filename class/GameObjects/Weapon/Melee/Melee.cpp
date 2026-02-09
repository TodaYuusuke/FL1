#include "Melee.h"
#include "../../Attack/AttackManager.h"
#include "../../Collision/CollisionMask.h"
#include "../../Camera/Effect/CameraEffectHandler.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace FLMath;

using namespace FLMath;

Melee::Melee(WeaponData gunData)
	: IMelee(gunData)
{
}

void Melee::Attack(int bulletHitFragBit, int bulletBelongFragBit, Actor* attackTarget)
{
	// 弾がない状態なら撃てない
	if (magazine_->GetEmpty()) { return; }
	// 射撃できる状態か
	if (!GetIsEnableAttack()) { return; }

	// 攻撃判定生成
	pBulletManager_->CreateAttack(data_.bulletType, &body_.worldTF, this, bulletHitFragBit, bulletBelongFragBit, attackMultiply_);
	// 弾数を減らす
	magazine_->BulletDecrement();
	// 攻撃中
	isAttack_ = true;

	// アニメーション再生
	anim_.Play("Attack");

	// エミッタが存在する場合射撃エフェクトを生成
	if (attackEffectEmitter_ != nullptr) {
		attackEffectEmitter_->Emit();
	}

	assistPos_ = actor_->GetWorldTF()->GetWorldPosition();
	// 攻撃対象あり
	if (attackTarget) {
		// 攻撃対象との距離
		Vector3 dist = attackTarget->GetWorldTF()->GetWorldPosition() - actor_->GetWorldTF()->GetWorldPosition();
		float length = dist.Length();
		if (length >= assistOffset) {
			length -= assistOffset;
		}
		else length = 0.0f;

		if (length > assistRange) {
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
		assistPos_ += Vector3{ 0.0f,0.0f,2.0f } *LWP::Math::Matrix4x4::CreateRotateXYZMatrix(actor_->GetWorldTF()->rotation);
	}
}

void Melee::Destroy() {

}

void Melee::SetWeaponSide(int weaponSide) {
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

	// アニメーションロード
	anim_.LoadFullPath(modelName, &body_);
	// アニメーション再生
	anim_.Play("Idle", 0.0f);
}
