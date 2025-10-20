#pragma once
#include "../IWeapon.h"
#include "../Components/Magazine.h"
#include <memory>

/// <summary>
/// 近距離クラス
/// </summary>
class IMelee : public IWeapon {
public:
	// コンストラクタ
	IMelee(WeaponData gunData);
	// デストラクタ
	~IMelee() override = default;

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
	/// <summary>
	/// 破壊処理
	/// </summary>
	void Destroy() override;

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

private:// 調整項目
	WeaponData meleeData_;

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