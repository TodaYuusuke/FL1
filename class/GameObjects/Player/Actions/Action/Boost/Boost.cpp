#include "Boost.h"
#include "../../../PlayerConfig.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace FLMath;
using namespace LWP::Math;

Boost::Boost(const LWP::Math::Vector3& dirVec, const LWP::Math::Vector3& startPos) {
	stateName_ = "Boost";
	// 遷移可能な状態
	enableChangeState_ = Movement::SubAction::jump | Movement::SubAction::sliding | Movement::SubAction::boost;

	json_.Init(kJsonFileDirectoryPath + "Boost.json")
		.AddValue<float>("Time", &maxBoostTime)
		.AddValue<float>("MaxSpeed", &end_)
		.CheckJsonFile();

	easeTimer_.Start(maxBoostTime);

	Init();
}

void Boost::Init() {
	//currentFrame_ = maxBoostFrame;
}

void Boost::Update() {
	moveSpeed_ = LWP::Utility::Interp::LerpF(start_, end_, LWP::Utility::Easing::OutBack(easeTimer_.GetProgress()));
	vel_ = LWP::Utility::Interp::Exponential(vel_, boostDir_.Normalize(), easeRate);
	easeTimer_.Update();
}

void Boost::DebugGui() {
	if (ImGui::TreeNode("Boost")) {
		json_.DebugGUI();
		ImGui::TreePop();
	}
}
