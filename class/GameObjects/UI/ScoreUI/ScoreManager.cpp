#include "ScoreManager.h"

void ScoreCounter::Update() {
	static int32_t frame;

	float t = 0.4f;

	frame = 0;
	if (drawScore_ < sumScore_) {
		drawScore_+=int(float(sumScore_ - drawScore_)*t)+1;
	}
	if (drawScore_ >= sumScore_) {
		drawScore_ = sumScore_;
	}
	
}