#include "MoveController.h"
#include "Action/Move/Move.h"
#include "Action/Evasion/Evasion.h"
#include "Action/None/NoneAction.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace LWP::Input;

MoveController::MoveController() {
	actions_[ActionType::kMain] = std::make_unique<Move>();
	actions_[ActionType::kSub] = std::make_unique<NoneAction>();
	enableChangeState_ = Movement::SubAction::jump | Movement::SubAction::sliding | Movement::SubAction::evasion;
}

MoveController::~MoveController() {

}

void MoveController::Init() {
	for (auto it = actions_.begin(); it != actions_.end(); ++it) {
		if(it->second) it->second->Init();
	}
}

void MoveController::Update() {
	// 入力処理
	InputHandle();

	for (auto it = actions_.begin(); it != actions_.end(); ++it) {
		if (it->second) it->second->Update();
	}
	vel_ = actions_[ActionType::kMain]->GetVel();
	rot_ = actions_[ActionType::kMain]->GetRot();
}

void MoveController::DebugGui() {
	for (auto it = actions_.begin(); it != actions_.end(); ++it) {
		if (it->second) it->second->DebugGui();
	}

	//Novice::ScreenPrintf((int)pos.x, (int)pos.y, "Move State:");

	//if (actions_[ActionType::kMain]) {
	//	// メイン
	//	Novice::ScreenPrintf((int)pos.x, (int)pos.y + 20, actions_[ActionType::kMain]->GetStateName().c_str());
	//}
	//if (actions_[ActionType::kSub]) {
	//	// サブ
	//	Novice::ScreenPrintf((int)pos.x, (int)pos.y + 40, actions_[ActionType::kSub]->GetStateName().c_str());
	//}
}

void MoveController::InputHandle() {
	// 回避
	if (Keyboard::GetTrigger(DIK_LSHIFT)) {
		if (CheckEnableChangeState(Movement::SubAction::evasion, actions_[ActionType::kSub]->GetEnableChangeState())) {
			if (CheckInabilityParallelState(Movement::SubAction::evasion, actions_[ActionType::kMain]->GetInabilityParallelState())) {
				ChangeState(actions_[ActionType::kSub], std::make_unique<Evasion>());
			}
		}
	}
}