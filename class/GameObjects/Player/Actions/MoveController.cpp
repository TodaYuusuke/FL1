#include "MoveController.h"
#include "Action/Move/Move.h"
#include "Action/Evasion/Evasion.h"
#include "Action/None/NoneAction.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/InputMyController/ControllerReceiver.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace LWP::Input;
using namespace LWP::Math;

MoveController::MoveController(Actor* actor) {
	pActor_ = actor;

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

	Vector3 vel{};
	// サブの行動が終了したら状態変更
	if (actions_[ActionType::kSub]->GetCurrentFrame() <= 0.0f) {
		ChangeState(actions_[ActionType::kSub], std::make_unique<NoneAction>());
	}
	// 回避時
	else if(actions_[ActionType::kSub]->GetStateName() == "Evasion") {
		vel = actions_[ActionType::kSub]->GetVel() - pActor_->GetWorldTF()->GetWorldPosition();
	}
	else {
		vel = actions_[ActionType::kMain]->GetVel() + actions_[ActionType::kSub]->GetVel();
	}

	vel_ = vel;
	rot_ = actions_[ActionType::kMain]->GetRot();
}

void MoveController::DebugGui() {
	for (auto it = actions_.begin(); it != actions_.end(); ++it) {
		if (it->second) it->second->DebugGui();
	}
}

void MoveController::InputHandle() {
	// 回避
	bool isEvasion = Keyboard::GetTrigger(DIK_LSHIFT) || Pad::GetTrigger(XBOX_A);
	if (ControllerReceiver::GetInstance()->IsOpen()) {
		isEvasion = Keyboard::GetTrigger(DIK_SPACE);
	}
	if (isEvasion) {
		if (CheckEnableChangeState(Movement::SubAction::evasion, actions_[ActionType::kSub]->GetEnableChangeState())) {
			if (CheckInabilityParallelState(Movement::SubAction::evasion, actions_[ActionType::kMain]->GetInabilityParallelState())) {
				ChangeState(actions_[ActionType::kSub], std::make_unique<Evasion>(actions_[ActionType::kMain]->GetVel(), pActor_->GetWorldTF()->GetWorldPosition()));
			}
		}
	}
}