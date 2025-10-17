#pragma once
#include "../IGun.h"
#include <memory>

/// <summary>
/// ライフルクラス
/// </summary>
class Rifle : public IGun {
public:
	// コンストラクタ
	Rifle(GunData gunData);
	// デストラクタ
	~Rifle() override = default;

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