#pragma once
#include <Adapter.h>

class BackGround
{
public:
	BackGround() { Initialize(); };
	~BackGround() {};

	void Initialize();
	void SetColor(const LWP::Utility::Color& color) { sprite_.material.color = color; };
private:

	// 背景スプライト
	LWP::Primitive::NormalSprite sprite_;
};