#pragma once
#include "../IGun.h"
#include <memory>

/// <summary>
/// ミサイルクラス
/// </summary>
class Missle : public IGun {
public:
	// コンストラクタ
	Missle(GunData gunData);
	// デストラクタ
	~Missle() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack() override;
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
	// リロードの経過時間
	float coolTime_;
	float burstInterval_;
};