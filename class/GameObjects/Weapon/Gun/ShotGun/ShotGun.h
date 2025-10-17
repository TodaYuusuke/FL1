#pragma once
#include "../IGun.h"
#include <memory>

/// <summary>
/// ショットガンクラス
/// </summary>
class ShotGun : public IGun {
public:
	// コンストラクタ
	ShotGun(GunData gunData);
	// デストラクタ
	~ShotGun() override = default;

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