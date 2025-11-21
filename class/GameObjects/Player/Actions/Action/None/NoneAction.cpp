#include "NoneAction.h"

using namespace FLMath;
using namespace ActionConfig;

NoneAction::NoneAction() {
	stateName_ = "None";
	// 遷移可能な状態
	enableChangeState_ = Mask::all;

	currentFrame_ = 1.0f;
}

void NoneAction::Init() {

}

void NoneAction::Update() {

}

void NoneAction::DebugGui() {
	if (ImGui::TreeNode("None")) {

		ImGui::TreePop();
	}
}
