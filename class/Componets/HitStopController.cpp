#include "HitStopController.h"

using namespace LWP;
using namespace LWP::Utility;
using namespace LWP::Information;

HitStopController::HitStopController() {
	// 現在のフレームレート算出
	fps = 1.0f / LWP::Info::GetDeltaTimeF();
	Init();
}

void HitStopController::Init() {
	// ヒットストップする時間
	hitStopTime_ = 0.0f;
	// 経過時間
	currentFrame_ = 0.0f;
	// 終了しているかを取得
	isFinish_ = true;
}

void HitStopController::Update() {
	// 現在のフレームレート算出
	if (LWP::Info::GetDeltaTimeF() <= 0.0f) {
		fps = 0.0f;
	}
	else {
		fps = 1.0f / LWP::Info::GetDefaultDeltaTimeF();;
	}

	// 数値が入っているならデクリメント
	if (currentFrame_ > 0.0f) {
		currentFrame_--;
	}

	// クールタイムが終了している
	if (currentFrame_ <= 0.0f) {
		// デルタタイムの係数初期化(追記:デルタタイムが0.0f以下の場合もセット)
		if (!isFinish_ or LWP::Info::GetDeltaTimeF() <= 0.0f) {
			SetDeltaTimeMultiply(1.0f);
		}
		isFinish_ = true;
	}
	else {
		isFinish_ = false;
	}
}

void HitStopController::DebugGui() {
	if (ImGui::BeginTabItem("HitStop")) {
		static float stopSecond;
		static float timeScale;
		if (ImGui::Button("Start")) {
			Start(stopSecond, timeScale);
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			Init();
		}

		ImGui::DragFloat("StopTime", &stopSecond, 0.001f, 0.0f);
		ImGui::DragFloat("Multiply", &timeScale, 0.001f, 0.0f);

		ImGui::EndTabItem();
	}
}