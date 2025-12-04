#include "GamePadDevice.h"

using namespace InputConfig;
using namespace LWP::Math;
using namespace LWP::Input;

GamePadDevice::GamePadDevice() {
	bindData_[(int)BindActionType::kCollect] = Command::GamePad::collect;
	bindData_[(int)BindActionType::kLeftHand] = Command::GamePad::leftHand;
	bindData_[(int)BindActionType::kRightHand] = Command::GamePad::rightHand;
	bindData_[(int)BindActionType::kLeftShoulder] = Command::GamePad::leftShoulder;
	bindData_[(int)BindActionType::kRightShoulder] = Command::GamePad::rightShoulder;
	bindData_[(int)BindActionType::kBoost] = Command::GamePad::boost;
	bindData_[(int)BindActionType::kInteract] = Command::GamePad::interact;
	bindData_[(int)BindActionType::kCancel] = Command::GamePad::cancel;
}

void GamePadDevice::Update() {

}

bool GamePadDevice::GetPress(BindActionType code) const {
	return Pad::GetPress(bindData_[(int)code]);
}

bool GamePadDevice::GetTrigger(BindActionType code) const {
	return Pad::GetTrigger(bindData_[(int)code]);
}

bool GamePadDevice::GetRelease(BindActionType code) const {
	return Pad::GetRelease(bindData_[(int)code]);
}

Vector2 GamePadDevice::GetLAxis() {
	Vector2 result = Pad::GetLStick();
	return result;
}

Vector2 GamePadDevice::GetRAxis() {
	Vector2 result = Pad::GetRStick();
	return result;
}
