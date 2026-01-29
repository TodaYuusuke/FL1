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

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:
};