#pragma once

#include <Adapter.h>

class ScoreUI
{
public:
	ScoreUI() {};
	~ScoreUI() {};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="num">桁数</param>
	void Initialize(size_t num);

	void SetCenter(LWP::Math::Vector2 center) { center_ = center; };
	void SetScore(int32_t num) { score_ = num; };

	void Update();

private:

	/// <summary>
	/// 数値からClipを設定する
	/// </summary>
	/// <param name="num">0~9の整数</param>
	//void SetClip(size_t digit, int num);

	//各桁の算出
	void SetNumbers();

	void CalcTranslate();

	static const size_t kMaxNum_ = 20;

	//表示スコア
	int32_t score_;

	size_t digit_ = 0;

	const LWP::Math::Vector2 kTextureSize_ = {64.0f,64.0f};

	LWP::Math::Vector2 center_ = {0,0};
	LWP::Math::Vector2 width_ = {10.0f,10.0f};

	LWP::Primitive::SequenceSprite sprites_[kMaxNum_];
};
