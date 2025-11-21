#include "Evasion.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace FLMath;
using namespace LWP::Math;

Evasion::Evasion(const LWP::Math::Vector3& dirVec, const LWP::Math::Vector3& startPos) {
	stateName_ = "Evasion";
	// 遷移可能な状態
	enableChangeState_ = Movement::SubAction::jump | Movement::SubAction::sliding;

	startPos_ = startPos;
	Vector3 pos = Vector3{ 0.0f,0.0f,maxEvasionDist } *Matrix4x4::CreateRotateXYZMatrix(LookRotationZLock(dirVec));
	endPos_ = startPos_ + (pos);

	Init();
}

void Evasion::Init() {
	currentFrame_ = maxEvasionFrame;
}

void Evasion::Update() {
	vel_ = LWP::Utility::Interp::Lerp(startPos_, endPos_, LWP::Utility::Easing::OutExpo((maxEvasionFrame - currentFrame_) / (maxEvasionFrame)));

	currentFrame_ -= HitStopController::GetInstance()->GetDeltaTime();
}

void Evasion::DebugGui() {
	if (ImGui::TreeNode("Evasion")) {

		ImGui::TreePop();
	}
}
