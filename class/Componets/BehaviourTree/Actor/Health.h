#pragma once
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <Adapter.h>

/// <summary>
/// HPクラス
/// </summary>
class Health {
public:
	// コンストラクタ
	Health(float maxHp);
	// デストラクタ
	~Health();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

public:// アクセサ
	/// <summary>
	/// ダメージ開始
	/// </summary>
	/// <param name="value">減少量</param>
	void Damage(float value);
	void Damage(float value, const std::string& name);

#pragma region Getter
	/// <summary>
	/// ダメージを与えた相手リストを取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::string> GetDamageAttackerName() { return damageAttackerName_; }
	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns></returns>
	float GetHealth() { return hp_; };
	/// <summary>
	/// 最大HPを取得
	/// </summary>
	/// <returns></returns>
	float GetMaxHealth() { return maxHp_; }
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
	/// <summary>
	/// ダメージを与えた相手の名前を設定
	/// </summary>
	/// <param name="name"></param>
	void SetAttackerName(const std::string& name) { damageAttackerName_.push_back(name); }
	/// <summary>
	/// 被弾しているかを設定
	/// </summary>
	/// <param name="isHit"></param>
	void SetIsHit(bool isHit) { isHit_ = isHit; }
#pragma endregion

private:
	float minHp_ = 0;
	float maxHp_;

private:
	float hp_;

	// ダメージを与えた相手の名前(重複回避)
	std::vector<std::string> damageAttackerName_;

	bool isHit_ = false;
};
