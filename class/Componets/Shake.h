#pragma once
#include "../Adapter/Adapter.h"
#include <random>
#include <variant>

/// <summary>
/// 揺れを引き起こす
/// </summary>
class Shake {
public:
	Shake() = default;
	~Shake() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:
	// 揺れの範囲をもとにランダム値を生成
	LWP::Math::Vector3 GenerateRandomShake(LWP::Math::Vector3 range) {
		return { GenerateRandomShake(range.x), GenerateRandomShake(range.y), GenerateRandomShake(range.z) };
	}
	float GenerateRandomShake(float range) {
		static std::random_device rd;
		static std::mt19937 mt(rd());

		std::uniform_real_distribution<float> dist(-range, range);
		return dist(mt);
	}

public:// アクセサ
#pragma region Setter
	/// <summary>
	/// 揺らす対象を設定
	/// </summary>
	/// <param name="target">揺らす対象</param>
	void SetTarget(LWP::Math::Vector3* target) {
		target_ = target;
		originalValue_ = *target_;
	}
	/// <summary>
	/// 揺れる範囲を設定
	/// </summary>
	/// <param name="range">揺れる範囲</param>
	void SetRange(LWP::Math::Vector3 range) { range_ = range; }
	/// <summary>
	/// 揺れの終了時間を設定
	/// </summary>
	/// <param name="endFrame">終了時間</param>
	void SetEndFrame(float endFrame) {
		endFrame_ = endFrame;
		currentFrame_ = endFrame_;
	}
	/// <summary>
	/// 揺れているかを設定
	/// </summary>
	/// <param name="isActive">揺らすか</param>
	void SetIsActive(bool isActive) { isActive_ = isActive; }
#pragma endregion

#pragma region Getter
	/// <summary>
	/// 揺れの値を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetValue() { return value_; }
	/// <summary>
	/// 揺れる範囲を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3* GetRange() { return &range_; }
	/// <summary>
	/// 経過時間を取得
	/// </summary>
	/// <returns></returns>
	float GetCurrentFrame() { return currentFrame_; }
	/// <summary>
	/// 揺れの終了時間を取得
	/// </summary>
	/// <returns></returns>
	float GetEndFrame() { return endFrame_; }
	/// <summary>
	/// 揺れているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() { return isActive_; }
	/// <summary>
	/// 終了しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsEnd() { return isEnd_; }
#pragma endregion

private:
	// 揺らす対象
	LWP::Math::Vector3* target_;
	// 対象が揺れていないときの値
	LWP::Math::Vector3 originalValue_;

	// 揺れた時の値
	LWP::Math::Vector3 value_;

	// 揺れの範囲
	LWP::Math::Vector3 range_;

	// 揺れていないとき値の補正値
	LWP::Math::Vector3 offset_;

	// 経過時間
	float currentFrame_;
	// 揺れの終了時間
	float endFrame_;

	// 揺らすか
	bool isActive_;
	// 終了しているか
	bool isEnd_;
};