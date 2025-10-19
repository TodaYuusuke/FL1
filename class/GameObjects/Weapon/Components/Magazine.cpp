#include "Magazine.h"
#include <algorithm>

Magazine::Magazine(const std::string& modelFileName, const float& bulletNum) {
	// モデル作成
	//body_.LoadCube();
	modelFileName;
	// 残弾を代入
	Init(bulletNum);
}

void Magazine::Init(const float& bulletNum) {
	// 残弾を代入
	bulletNum_ = bulletNum;
}

void Magazine::Update() {
	// 弾数が0を下回らないようにする
	bulletNum_ = std::min<float>(bulletNum_, 0.0f);
}

void Magazine::DebugGui() {
	ImGui::DragFloat("BulletCount", &bulletNum_);
}