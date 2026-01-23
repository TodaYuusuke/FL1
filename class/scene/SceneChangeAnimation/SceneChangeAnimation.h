#pragma once

#include "Adapter.h"

class DefaultSceneChangeAnimation
{
public:
	DefaultSceneChangeAnimation() {};
	~DefaultSceneChangeAnimation() {};

	void Initialize();

	/// <summary>
	/// アニメーション開始
	/// </summary>
	/// <param name="type">0:in 1:out</param>
	void Start(int type);

	void Update();

	bool GetIsPlay() { return isPlay_; };

	void SetAnimationLength(int length) { animationLength_ = length; };

private:
	const LWP::Math::Vector2 kTextureSize_ = { 480.0f,270.0f };
	static const size_t kSpritFrame_ = 7;//アニメーション分割数
	static const size_t kSpriteNum_ = 4;
	std::array<LWP::Primitive::SequenceSprite, kSpriteNum_> sprites_;
	int frame_;
	int rect_;//再生向き
	bool isPlay_;
	int animationLength_;//アニメーション再生時間(フレーム)
	int type_;
};
