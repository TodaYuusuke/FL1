#include "NoneAction.h"

using namespace FLMath;
using namespace ActionConfig;
using namespace LWP;
using namespace LWP::Math;

NoneAction::NoneAction(const Vector3& vel, float speed) {
	stateName_ = "None";
	vel_ = vel;
	moveSpeed_ = speed;

	// 遷移可能な状態
	enableChangeState_ = Mask::all;

	currentFrame_ = 1.0f;
}

void NoneAction::Init() {

}

void NoneAction::Update() {
	moveSpeed_ = FLMath::Exponential(moveSpeed_, 1.0f, easeRate);
	vel_ = Utility::Interp::Exponential(vel_, Vector3{0.0f,0.0f,0.0f}, easeRate);
}

void NoneAction::DebugGui() {
	if (ImGui::TreeNode("None")) {

		ImGui::TreePop();
	}
}
