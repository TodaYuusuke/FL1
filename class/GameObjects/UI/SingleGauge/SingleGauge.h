#pragma once
#include <Adapter.h>

class  SingleGauge
{
public:
	SingleGauge() {};
	~SingleGauge() {};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="type">増減アニメーション有無</param>
	void Initialize(bool type);

	void Update();

	void SetAnchor(LWP::Math::Vector3 anchor) { anchor_ = anchor; };
	void SetSize(LWP::Math::Vector3 size) { size_ = size; };
	void SetRotate(float rotate) { rotate_ = rotate; };

	void SetRatio(float ratio) { ratio_ = ratio; };
	void SetColor(const LWP::Utility::Color& color) { gaugeSprite_.material.color = color; };

private:
	LWP::Primitive::NormalSprite backSprite_;
	LWP::Primitive::NormalSprite gaugeSprite_;
	LWP::Primitive::NormalSprite differenceSprite_;

	LWP::Math::Vector3 size_ = {0.5f,0.5f,1.0f};
	LWP::Math::Vector3 anchor_ = { 0,0,0 };

	float rotate_;

	float ratio_;
	bool type_ = false;
	float preRatio_;
	float decay_ = 0.001f;
};
