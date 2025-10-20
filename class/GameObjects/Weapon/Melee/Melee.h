#pragma once
#include "IMelee.h"
#include <memory>

/// <summary>
/// 近距離攻撃クラス
/// </summary>
class Melee : public IMelee {
public:
	// コンストラクタ
	Melee(WeaponData gunData);
	// デストラクタ
	~Melee() override = default;

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