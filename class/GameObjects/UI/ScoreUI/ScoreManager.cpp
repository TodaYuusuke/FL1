#include "ScoreManager.h"

void ScoreCounter::Update() {
	static int32_t frame;
	if (++frame > incrementFrame_) {
		frame = 0;
		if (drawScore_ < sumScore_) {
			drawScore_++;
		}
	}
}