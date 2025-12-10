#include "KeyboardDevice.h"

using namespace InputConfig;
using namespace LWP::Math;
using namespace LWP::Input;

KeyboardDevice::KeyboardDevice() {
	bindData_[(int)BindActionType::kCollect]		= Command::Key::collect;
	bindData_[(int)BindActionType::kLeftHand]		= Command::Key::leftHand;
	bindData_[(int)BindActionType::kRightHand]		= Command::Key::rightHand;
	bindData_[(int)BindActionType::kLeftShoulder]	= Command::Key::leftShoulder;
	bindData_[(int)BindActionType::kRightShoulder]	= Command::Key::rightShoulder;
	bindData_[(int)BindActionType::kBoost]			= Command::Key::boost;
	bindData_[(int)BindActionType::kInteract]		= Command::Key::interact;
	bindData_[(int)BindActionType::kCancel]			= Command::Key::cancel;
}

void KeyboardDevice::Update() {

}

bool KeyboardDevice::PressAnyKey() {
	return Keyboard::GetPress(DIK_SPACE);
}

bool KeyboardDevice::TriggerAnyKey() {
	return Keyboard::GetTrigger(DIK_SPACE);
}

bool KeyboardDevice::GetPress(BindActionType code) const {
	return Keyboard::GetPress(bindData_[(int)code]);
}

bool KeyboardDevice::GetTrigger(BindActionType code) const {
	return Keyboard::GetTrigger(bindData_[(int)code]);
}

bool KeyboardDevice::GetRelease(BindActionType code) const {
	return Keyboard::GetRelease(bindData_[(int)code]);
}

Vector2 KeyboardDevice::GetLAxis() {
	Vector2 result{};
	if (Keyboard::GetPress(InputConfig::Command::Key::Move::Right)) {
		result.x += 1.0f;
	}
	if (Keyboard::GetPress(InputConfig::Command::Key::Move::Left)) {
		result.x += -1.0f;
	}
	if (Keyboard::GetPress(InputConfig::Command::Key::Move::Up)) {
		result.y += 1.0f;
	}
	if (Keyboard::GetPress(InputConfig::Command::Key::Move::Down)) {
		result.y += -1.0f;
	}

	// 斜め入力の場合正規化
	if (result.Length() > 1.0f) result = result.Normalize();

	return result;
}

Vector2 KeyboardDevice::GetRAxis() {
	Vector2 result{};
	if (Keyboard::GetPress(InputConfig::Command::Key::CameraMove::Right)) {
		result.x += 1.0f;
	}
	if (Keyboard::GetPress(InputConfig::Command::Key::CameraMove::Left)) {
		result.x += -1.0f;
	}
	if (Keyboard::GetPress(InputConfig::Command::Key::CameraMove::Up)) {
		result.y += 1.0f;
	}
	if (Keyboard::GetPress(InputConfig::Command::Key::CameraMove::Down)) {
		result.y += -1.0f;
	}

	// 斜め入力の場合正規化
	if (result.Length() > 1.0f) result = result.Normalize();

	return result;
}
