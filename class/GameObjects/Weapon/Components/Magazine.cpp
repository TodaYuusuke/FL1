#include "Magazine.h"
#include <algorithm>

Magazine::Magazine(const int& bulletNum) {
	// 残弾を代入
	Init(bulletNum);
}

void Magazine::Init(const int& bulletNum) {
	// 残弾を代入
	bulletNum_ = bulletNum;
}

void Magazine::Update() {
	// 弾数が0を下回らないようにする
	bulletNum_ = std::min<int>(bulletNum_, 0);
}

void Magazine::DebugGui() {
	ImGui::DragInt("BulletCount", &bulletNum_);
}