#include "Evasion.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;

Evasion::Evasion() {
	stateName_ = "Evasion";
	// 遷移可能な状態
	enableChangeState_ = Movement::SubAction::jump | Movement::SubAction::sliding;
}

void Evasion::Init() {

}

void Evasion::Update() {

}

void Evasion::DebugGui() {
	if (ImGui::TreeNode("Evasion")) {

		ImGui::TreePop();
	}
}
