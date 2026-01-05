#pragma once
#include <Adapter.h>
#include "UI/ScoreUI/ScoreUI.h"

/// <summary>
/// ウェーブの管理クラス
/// </summary>
class WaveManager final : public LWP::Utility::ISingleton<WaveManager> {
	friend class LWP::Utility::ISingleton<WaveManager>;
private:
	// コンストラクタ
	WaveManager();
	// デストラクタ
	~WaveManager();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:
	/// <summary>
	/// ウェーブ遷移演出
	/// </summary>
	void TransitionEffect();

public:
	/// <summary>
	/// 次のウェーブ開始
	/// </summary>
	void StartNextWave();

#pragma region Getter
	/// <summary>
	/// 次のウェーブに遷移している最中かを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsTransitionNextWave() { return isTransitionWave_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:// 定数
	const std::string kDirectoryPath = "WaveUI/";

private:// 調整項目
	// 遷移時間
	float transitionEffectTime = 2.0f;

	LWP::Utility::JsonIO json_;

private:
	// 遷移スプライト
	LWP::Primitive::NormalSprite transitionSprite_;
	// 数字スプライト
	std::unique_ptr<ScoreUI> numberUI_;

	LWP::Math::Vector2 screenCenterPos_;
	std::map<std::string, LWP::Math::Vector2> centerPos_;

	// 現在のウェーブ数
	int currentWaveNum_;

	// 遷移時間
	float currentFrame_;

	// ウェーブ遷移中か
	bool isTransitionWave_;
};

