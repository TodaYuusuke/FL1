#include "Timer.h"

void Timer::Init() {
	currentFrame_ = 0.0f;
	isActive_ = false;
	isEnd_ = false;
}

void Timer::Update() {
	if (!isActive_) { return; }

	if (currentFrame_ <= 0.0f) {
		isEnd_ = true;
	}
	else {
		isEnd_ = false;
	}

	currentFrame_--;
}

void Timer::Start(const float& frame) {
	currentFrame_ = frame;
	isActive_ = true;
	isEnd_ = false;
}
