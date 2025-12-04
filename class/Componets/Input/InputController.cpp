#include "InputController.h"
#include "Device/KeyboardDevice.h"
#include "Device/GamePadDevice.h"

InputController::InputController() {
	device_.push_back(std::make_unique<KeyboardDevice>());
	device_.push_back(std::make_unique<GamePadDevice>());
}

void InputController::Init() {

}

void InputController::Update() {
	for (int i = 0; i < device_.size(); i++) {
		device_[i]->Update();
	}
}