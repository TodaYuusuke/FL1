#pragma once
#include "../IGun.h"
#include <memory>

/// <summary>
/// ミサイルクラス
/// </summary>
class Missile : public IGun {
public:
	// コンストラクタ
	Missile(WeaponData gunData);
	// デストラクタ
	~Missile() override = default;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack(int bulletHitFragBit, int bulletBelongFragBit, Actor* attackTarget) override;
	/// <summary>
	/// 攻撃指示
	/// </summary>
	void AttackCommond() override;
	/// <summary>
	/// 落下処理
	/// </summary>
	void FallingUpdate() override;

	/// <summary>
	/// 武器の向き
	/// </summary>
	/// <param name="weaponSide"></param>
	void SetWeaponSide(int weaponSide = -1) override;

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:
	// リロードの経過時間
	float coolTime_;
	float burstInterval_;
};