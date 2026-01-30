#pragma once
#include "../IGun.h"
#include <memory>

/// <summary>
/// ショットガンクラス
/// </summary>
class ShotGun : public IGun {
public:
	// コンストラクタ
	ShotGun(WeaponData gunData);
	// デストラクタ
	~ShotGun() override = default;

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:
};