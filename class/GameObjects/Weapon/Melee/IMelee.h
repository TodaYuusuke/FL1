#pragma once
#include "../IWeapon.h"
#include "../Components/Magazine.h"
#include <memory>

/// <summary>
/// 近距離クラス
/// </summary>
class IMelee : public IWeapon {
public:
	// コンストラクタ
	IMelee(WeaponData data);
	// デストラクタ
	~IMelee() override;

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
	void Attack(int bulletHitFragBit, Actor* attackTarget = nullptr) override;
	/// <summary>
	/// リロード
	/// </summary>
	void Reload();
	/// <summary>
	/// 破壊処理
	/// </summary>
	void Destroy() override;

private:
	/// <summary>
	/// 位置アシスト
	/// </summary>
	void AttackAssist();

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:
	// 攻撃時のアシスト座標
	LWP::Math::Vector3 assistPos_;
};