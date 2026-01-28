#pragma once
#include "../IGun.h"
#include <memory>

/// <summary>
/// マシンガンクラス
/// </summary>
class MachineGun : public IGun {
public:
	// コンストラクタ
	MachineGun(WeaponData gunData);
	// デストラクタ
	~MachineGun() override = default;

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
};