#pragma once
#include <Adapter.h>

class BackGround
{
public:
	BackGround() { Initialize(); };
	~BackGround() {};

	void Initialize();

private:

	// 背景スプライト
	LWP::Primitive::NormalSprite sprite_;
};