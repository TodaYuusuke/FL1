#pragma once
#include "../IWeapon.h"
#include "../../Bullets/BulletManager.h"
#include <memory>

/// <summary>
/// 銃クラス
/// </summary>
class IGun : public IWeapon {
public:
	// コンストラクタ
	IGun(WeaponData gunData);
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
	void DebugGui();

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

#pragma endregion

#pragma region Setter

#pragma endregion

private:
	// 
	BulletManager* pBulletManager_;

private:// 調整項目
	WeaponData gunData_;

private:
};