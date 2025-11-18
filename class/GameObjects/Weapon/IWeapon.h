#pragma once
#include "WeaponConfig.h"
#include "Components/Magazine.h"
#include "../../Componets/Math.h"
#include "../../Componets/HitStopController.h"
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include "../Bullets/BulletManager.h"
#include <Adapter.h>
#include <string>

/// <summary>
/// 武器クラスのインターフェイス
/// </summary>
class IWeapon {
public:
	// コンストラクタ
	IWeapon() = default;
	// デストラクタ
	virtual ~IWeapon() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 調整項目
	/// </summary>
	virtual void DebugGui() = 0;
	/// <summary>
	/// 攻撃
	/// </summary>
	/// <param name="bulletHitFragBit">弾の当たり判定をとるビット</param>
	/// <param name="attackTarget">被攻撃対象(位置アシストをしたい場合代入)</param>
	virtual void Attack(int bulletHitFragBit, Actor* attackTarget = nullptr) = 0;
	/// <summary>
	/// 破壊処理
	/// </summary>
	virtual void Destroy() = 0;

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 所持者の取得
	/// </summary>
	/// <returns></returns>
	Actor* GetActor() { return actor_; }
	/// <summary>
	/// モデルを取得
	/// </summary>
	/// <returns></returns>
	LWP::Resource::SkinningModel GetModel() { return body_; }
	/// <summary>
	/// 武器情報を取得
	/// </summary>
	/// <returns></returns>
	WeaponData GetWeaponData() { return data_; }
	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::TransformQuat* GetWorldTF() { return &body_.worldTF; }
	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns></returns>
	std::string GetName() { return name_; }
	/// <summary>
	/// 攻撃不可時間中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsCoolTime() { return coolFrame_ > 0.0f; }
	/// <summary>
	/// リロード中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsReloadTime() { return reloadFrame_ > 0.0f; }
	/// <summary>
	/// 銃を撃つまでの溜め時間中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsStoreTime() { return storeFrame_ > 0.0f; }
	/// <summary>
	/// 射撃できるかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsEnableAttack() { return attackFrame_ <= 0.0f; }
	/// <summary>
	/// 破壊するかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDestroy() { return isDestroy_; }
	/// <summary>
	/// 攻撃を行っているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAttack() { return isAttack_; }
	/// <summary>
	/// 弾切れかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsEmpty() { return magazine_->GetEmpty(); }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 親子付けの設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(Actor* character) {
		actor_ = character;
		if (!actor_) {
			body_.worldTF.ClearParent();
		}
		else {
			body_.worldTF.Parent(character->GetWorldTF());
		}
	}
	/// <summary>
	/// 弾発射時の方向ベクトルを設定
	/// </summary>
	/// <param name="dirVel"></param>
	/// <returns></returns>
	void SetShotDirVelocity(const LWP::Math::Vector3& dirVel) { shotDirVel_ = dirVel; }
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="translation"></param>
	void SetTranslation(const LWP::Math::Vector3& translation) { body_.worldTF.translation = translation; }
	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="q"></param>
	void SetRotation(const LWP::Math::Quaternion& q) { body_.worldTF.rotation = q; }
	/// <summary>
	/// 攻撃倍率を設定
	/// </summary>
	/// <param name="multiply"></param>
	void SetAttackMultiply(float multiply) { attackMultiply_ = multiply; }
	/// <summary>
	/// 速度倍率を設定
	/// </summary>
	/// <param name="multiply"></param>
	void SetSpeedMultiply(float multiply) { speedMultiply_ = multiply; }
	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="name"></param>
	void SetName(const std::string& name) { name_ = name; }
#pragma endregion

protected:
	// ヒットストップ
	HitStopController* stopController_;
	// 弾管理クラスのアドレス
	BulletManager* pBulletManager_;

	// モデル
	LWP::Resource::SkinningModel body_;

	// 弾倉
	std::unique_ptr<Magazine> magazine_;

	// 武器情報
	WeaponData data_;

	// 所有者
	Actor* actor_;

	// 名前
	std::string name_;

	// 弾発射時の方向ベクトル
	LWP::Math::Vector3 shotDirVel_;

	// 攻撃力
	float currentAttackValue_;
	// 攻撃倍率
	float attackMultiply_ = 1.0f;
	// 速度倍率
	float speedMultiply_ = 1.0f;

	// 射撃時の経過時間
	float attackFrame_;
	// 使用不可の経過時間
	float coolFrame_;
	// リロード時間
	float reloadFrame_;
	// 銃を撃つまでの溜め時間
	float storeFrame_;

	// 破壊するか
	bool isDestroy_;
	// 攻撃するか
	bool isAttack_;
};

