#include "VirtualController.h"
#include "Device/KeyboardDevice.h"
#include "Device/GamePadDevice.h"

using namespace LWP::Math;
using namespace LWP::Input;

VirtualController::VirtualController() {
	device_.push_back(std::make_unique<KeyboardDevice>());
	device_.push_back(std::make_unique<GamePadDevice>());
}

void VirtualController::Update() {
	for (int i = 0; i < device_.size(); i++) {
		device_[i]->Update();
	}
}

bool VirtualController::GetPress(BindActionType actionType) const {
	bool result = false;
	for (int i = 0; i < device_.size(); i++) {
		if (device_[i]->GetPress(actionType)) result = true;
	}
	return result;
}

bool VirtualController::GetTrigger(BindActionType actionType) const {
	bool result = false;
	for (int i = 0; i < device_.size(); i++) {
		if (device_[i]->GetTrigger(actionType)) result = true;
	}
	return result;
}

bool VirtualController::GetRelease(BindActionType actionType) const {
	bool result = false;
	for (int i = 0; i < device_.size(); i++) {
		if (device_[i]->GetRelease(actionType)) result = true;
	}
	return result;
}

Vector2 VirtualController::GetLAxis() const {
	Vector2 result{};
	for (int i = 0; i < device_.size(); i++) result += device_[i]->GetLAxis();

	return result.Normalize();
}

Vector2 VirtualController::GetRAxis() const {
	Vector2 result{};
	for (int i = 0; i < device_.size(); i++) result += device_[i]->GetRAxis(); 

	return result.Normalize();
}
