#pragma once

/// <summary>
/// タイマー
/// </summary>
class Timer {
public:
	// コンストラクタ
	Timer() = default;
	// デストラクタ
	~Timer() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

public:// アクセサ
	/// <summary>
	/// カウント開始
	/// </summary>
	void Start(const float& frame);

#pragma region Getter
	/// <summary>
	/// 既定の時間をカウントし終えたかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsEnd() { return isEnd_; }
	/// <summary>
	/// 経過フレームを取得
	/// </summary>
	/// <returns></returns>
	bool GetCurrentFrame() { return currentFrame_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:
	// 経過時間
	float currentFrame_;

	// 活動状態か
	bool isActive_;
	bool isEnd_;
};

