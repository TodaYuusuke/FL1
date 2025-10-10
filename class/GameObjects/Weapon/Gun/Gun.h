#pragma once
#include "../IWeapon.h"
#include "Components/Magazine.h"
#include "../../Bullets/BulletManager.h"
#include <memory>

/// <summary>
/// 銃クラス
/// </summary>
class Gun : public IWeapon {
public:
	// コンストラクタ
	Gun(GunData gunData);
	// デストラクタ
	~Gun() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui() override;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack() override;
	/// <summary>
	/// リロード
	/// </summary>
	void Reload();

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// リロード中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsReloading() { return reloadFrame_ > 0.0f; }
	/// <summary>
	/// 射撃できるかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsEnableShot() { return shotFrame_ <= 0.0f; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:
	// 
	BulletManager* pBulletManager_;

private:// 調整項目
	GunData gunData_;

private:
	// 弾倉
	std::unique_ptr<Magazine> magazine_;

	// 攻撃力
	float currentAttackValue_;

	// 射撃時の経過時間
	float shotFrame_;
	// リロードの経過時間
	float reloadFrame_;
};