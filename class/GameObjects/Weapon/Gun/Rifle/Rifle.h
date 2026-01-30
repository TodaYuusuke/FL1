#pragma once
#include "../IGun.h"
#include <memory>

/// <summary>
/// ライフルクラス
/// </summary>
class Rifle : public IGun {
public:
	// コンストラクタ
	Rifle(WeaponData gunData);
	// デストラクタ
	~Rifle() override = default;

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:
};