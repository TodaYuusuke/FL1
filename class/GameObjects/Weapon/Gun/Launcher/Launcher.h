#pragma once
#include "../IGun.h"
#include <memory>

/// <summary>
/// ランチャークラス
/// </summary>
class Launcher : public IGun {
public:
	// コンストラクタ
	Launcher(WeaponData gunData);
	// デストラクタ
	~Launcher() override = default;

	/// <summary>
	/// 破壊処理
	/// </summary>
	void Destroy() override;

	void SetWeaponSide(int weaponSide = -1) override;

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:
};