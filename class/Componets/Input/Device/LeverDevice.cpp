#include "LeverDevice.h"

using namespace InputConfig;
using namespace LWP::Math;
using namespace LWP::Input;

LeverDevice::LeverDevice() {
	preKeys_ = ControllerReceiver::GetInstance()->GetData().stick.multiSticks;
}

void LeverDevice::Update() {
	preKeys_ = ControllerReceiver::GetInstance()->GetData().stick.multiSticks;

	// ポートが開いていないなら前の入力を初期化
	if (!ControllerReceiver::GetInstance()->IsOpen()) preKeys_ = {};
}

bool LeverDevice::PressAnyKey() {
	if (!ControllerReceiver::GetInstance()->IsOpen()) return false;
	GameUsedData::multiSticks::stick2Shaft result = ControllerReceiver::GetInstance()->GetData().stick.multiSticks;

	return Keyboard::GetPress(DIK_LSHIFT)						||	// 
		result.stickLeft.button0								||	// 
		result.stickRight.button0								||	// 
		result.stickLeft.button1								||	// 
		result.stickRight.button1								||	// 
		Keyboard::GetPress(InputConfig::Command::Key::boost);		// 
}

bool LeverDevice::TriggerAnyKey() {
	if (!ControllerReceiver::GetInstance()->IsOpen()) return false;
	GameUsedData::multiSticks::stick2Shaft result = ControllerReceiver::GetInstance()->GetData().stick.multiSticks;

	return (Keyboard::GetTrigger(DIK_LSHIFT))							||	// 
			(result.stickLeft.button0 && !preKeys_.stickLeft.button0)	||	// 
			(result.stickRight.button0 && !preKeys_.stickRight.button0) ||	// 
			(result.stickLeft.button1 && !preKeys_.stickLeft.button1)	||	// 
			(result.stickRight.button1 && !preKeys_.stickRight.button1) ||	// 
			(Keyboard::GetTrigger(InputConfig::Command::Key::boost));
}

bool LeverDevice::GetPress(BindActionType code) const {
	if (!ControllerReceiver::GetInstance()->IsOpen()) return false;

	GameUsedData::multiSticks::stick2Shaft result = ControllerReceiver::GetInstance()->GetData().stick.multiSticks;

	switch (code) {
	case BindActionType::kCollect:
		return Keyboard::GetPress(DIK_LSHIFT);
		break;
	case BindActionType::kLeftHand:
		return result.stickLeft.button0;
		break;
	case BindActionType::kRightHand:
		return result.stickRight.button0;
		break;
	case BindActionType::kLeftShoulder:
		return result.stickLeft.button1;
		break;
	case BindActionType::kRightShoulder:
		return result.stickRight.button1;
		break;
	case BindActionType::kBoost:
		return Keyboard::GetPress(InputConfig::Command::Key::boost);
		break;
	case BindActionType::kInteract:
		return false;
		break;
	case BindActionType::kCancel:
		return false;
		break;
	default:
		return false;
		break;
	}
}

bool LeverDevice::GetTrigger(BindActionType code) const {
	if (!ControllerReceiver::GetInstance()->IsOpen()) return false;

	GameUsedData::multiSticks::stick2Shaft result = ControllerReceiver::GetInstance()->GetData().stick.multiSticks;

	switch (code) {
	case BindActionType::kCollect:
		return Keyboard::GetTrigger(DIK_LSHIFT);
		break;
	case BindActionType::kLeftHand:
		return result.stickLeft.button0 && !preKeys_.stickLeft.button0;
		break;
	case BindActionType::kRightHand:
		return result.stickRight.button0 && !preKeys_.stickRight.button0;
		break;
	case BindActionType::kLeftShoulder:
		return result.stickLeft.button1 && !preKeys_.stickLeft.button1;
		break;
	case BindActionType::kRightShoulder:
		return result.stickRight.button1 && !preKeys_.stickRight.button1;
		break;
	case BindActionType::kBoost:
		return Keyboard::GetTrigger(InputConfig::Command::Key::boost);
		break;
	case BindActionType::kInteract:
		return false;
		break;
	case BindActionType::kCancel:
		return false;
		break;
	default:
		return false;
		break;
	}
}

bool LeverDevice::GetRelease(BindActionType code) const {
	if (!ControllerReceiver::GetInstance()->IsOpen()) return false;

	GameUsedData::multiSticks::stick2Shaft result = ControllerReceiver::GetInstance()->GetData().stick.multiSticks;

	switch (code) {
	case BindActionType::kCollect:
		return Keyboard::GetRelease(DIK_LSHIFT);
		break;
	case BindActionType::kLeftHand:
		return !result.stickLeft.button0 && preKeys_.stickLeft.button0;
		break;
	case BindActionType::kRightHand:
		return !result.stickRight.button0 && preKeys_.stickRight.button0;
		break;
	case BindActionType::kLeftShoulder:
		return !result.stickLeft.button1 && preKeys_.stickLeft.button1;
		break;
	case BindActionType::kRightShoulder:
		return !result.stickRight.button1 && preKeys_.stickRight.button1;
		break;
	case BindActionType::kBoost:
		return Keyboard::GetRelease(InputConfig::Command::Key::boost);
		break;
	case BindActionType::kInteract:
		return false;
		break;
	case BindActionType::kCancel:
		return false;
		break;
	default:
		return false;
		break;
	}
}

Vector2 LeverDevice::GetLAxis() {
	if (!ControllerReceiver::GetInstance()->IsOpen()) return Vector2{};

	Vector2 result = ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickLeft.lever;
	return result;
}

Vector2 LeverDevice::GetRAxis() {
	if (!ControllerReceiver::GetInstance()->IsOpen()) return Vector2{};

	Vector2 result = ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickRight.lever;
	return result;
}
