#pragma once
#include <Adapter.h>
#include <string>

/// <summary>
/// 残弾を管理する弾倉クラス
/// </summary>
class Magazine {
public:
	// コンストラクタ
	Magazine(const std::string& modelFileName, const float& bulletNum);
	// デストラクタ
	~Magazine() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(const float& bulletNum);
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
	/// 残弾を減らす
	/// </summary>
	/// <param name="value">減少量</param>
	void BulletDecrement(const float& value = 1) { bulletNum_ -= value; }

#pragma region Getter
	/// <summary>
	/// 弾数を取得
	/// </summary>
	/// <returns></returns>
	float GetBulletNum() { return bulletNum_; }

	/// <summary>
	/// 弾があるかを取得
	/// </summary>
	/// <returns></returns>
	bool GetEmpty() { return bulletNum_ <= 0.0f; }
#pragma endregion 

private:
	// モデル
	LWP::Resource::RigidModel body_;
	// 弾数
	float bulletNum_;
};

