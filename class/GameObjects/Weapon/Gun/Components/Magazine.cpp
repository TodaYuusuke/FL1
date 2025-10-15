#include "Magazine.h"
#include <algorithm>

Magazine::Magazine(const std::string& modelFileName, const float& bulletCount) {
	// モデル作成
	body_.LoadCube();
	modelFileName;
	// 残弾を代入
	Init(bulletCount);
}

void Magazine::Init(const float& bulletCount) {
	// 残弾を代入
	bulletCount_ = bulletCount;
}

void Magazine::Update() {
	// 弾数が0を下回らないようにする
	bulletCount_ = std::min<float>(bulletCount_, 0.0f);
}

void Magazine::DebugGui() {
	ImGui::DragFloat("BulletCount", &bulletCount_);
}