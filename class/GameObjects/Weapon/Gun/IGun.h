#pragma once
#include "../IWeapon.h"
#include <memory>

/// <summary>
/// 銃クラス
/// </summary>
class IGun : public IWeapon {
public:
	// コンストラクタ
	IGun(WeaponData data);
	// デストラクタ
	~IGun() override = default;

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
	void Attack(int bulletHitFragBit, int bulletBelongFragBit, Actor* attackTarget = nullptr) override;
	/// <summary>
	/// リロード
	/// </summary>
	void Reload();
	/// <summary>
	/// 破壊処理
	/// </summary>
	void Destroy() override;
	/// <summary>
	/// 落下処理
	/// </summary>
	void FallingUpdate() override;

protected:
	/// <summary>
	/// 攻撃指示
	/// </summary>
	virtual void AttackCommond();

	/// <summary>
	/// バースト状態にする
	/// </summary>
	void BurstMode();
	/// <summary>
	/// フルオート状態にする
	/// </summary>
	void FullAutoMode();

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter
	/// <summary>
	/// 武器の向きを設定
	/// </summary>
	/// <param name="weaponSide"></param>
	void SetWeaponSide(int weaponSide = -1) override { weaponSide; }
#pragma endregion

public: // 演出用関数群

	/// <summary>
	///  攻撃エフェクトの再生関数
	/// </summary>
	void PlayAttackEffect() override;

protected:
	Actor* target_;

	// 射撃スタイル
	ShotType shotType_;
	// 現在のバースト数
	int burstNum_;

	int bulletHitFragBit_;
	int bulletBelongFragBit_;
};