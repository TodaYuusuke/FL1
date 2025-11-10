#pragma once
#include <algorithm>

/// <summary>
/// HPクラス
/// </summary>
class Health {
public:
	// コンストラクタ
	Health(float maxHp);
	// デストラクタ
	~Health();

public:// アクセサ
	/// <summary>
	/// ダメージ開始
	/// </summary>
	/// <param name="value">減少量</param>
	void Damage(float value) { hp_ = std::clamp<float>(hp_ - value, minHp_, maxHp_); }

#pragma region Getter
	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns></returns>
	float GetHealth() { return hp_; };
	/// <summary>
	/// 死亡しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDead();
#pragma endregion

#pragma region Setter
	/// <summary>
	/// HPを設定
	/// </summary>
	/// <param name="health"></param>
	void SetHealth(float health) { hp_ = health; }
#pragma endregion

private:
	float minHp_ = 0;
	float maxHp_;

private:
	float hp_;
};
