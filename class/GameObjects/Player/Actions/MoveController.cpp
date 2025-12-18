#include "MoveController.h"
#include "Action/Move/Move.h"
#include "Action/Boost/Boost.h"
#include "Action/None/NoneAction.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/InputMyController/ControllerReceiver.h"

using namespace ActionConfig;
using namespace ActionConfig::Mask;
using namespace LWP::Input;
using namespace LWP::Math;

MoveController::MoveController(BlackBoard* blackBoard) {
	pBB_ = blackBoard;
	vCon_ = VirtualController::GetInstance();
	cameraEffector_ = CameraEffectHandler::GetInstance();

	actions_[ActionType::kMain] = std::make_unique<Move>(pBB_);
	actions_[ActionType::kSub] = std::make_unique<NoneAction>();
	enableChangeState_ = Movement::SubAction::jump | Movement::SubAction::sliding | Movement::SubAction::boost;
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
	// 回避時
	if(actions_[ActionType::kSub]->GetStateName() == "Boost") {
		vel = actions_[ActionType::kMain]->GetVel() * actions_[ActionType::kMain]->GetMoveSpeed() * actions_[ActionType::kSub]->GetMoveSpeed();
	}
	else {
		vel = actions_[ActionType::kMain]->GetVel() * actions_[ActionType::kMain]->GetMoveSpeed() + actions_[ActionType::kSub]->GetVel() * actions_[ActionType::kSub]->GetMoveSpeed();
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
	bool isBoost = false;
	if (vCon_->GetPress(BindActionType::kBoost)) {
		if (CheckEnableChangeState(Movement::SubAction::boost, actions_[ActionType::kSub]->GetEnableChangeState())) {
			if (CheckInabilityParallelState(Movement::SubAction::boost, actions_[ActionType::kMain]->GetInabilityParallelState())) {
				ChangeState(actions_[ActionType::kSub], std::make_unique<Boost>(actions_[ActionType::kMain]->GetVel(), pBB_->GetValue<Actor*>("Player")->GetWorldTF()->GetWorldPosition()));
				isBoost = true;

				// カメラ演出開始
				if (vCon_->GetTrigger(BindActionType::kBoost)) {
					cameraEffector_->StartZoom(boostCameraFov, boostCameraEffectTime);
					cameraEffector_->StartShake(boostCameraShake, boostCameraEffectTime);
					cameraEffector_->StartBound(boostCameraBound, boostCameraBoundTime);
				}
			}
		}
	}
	if (!isBoost) {
		ChangeState(actions_[ActionType::kSub], std::make_unique<NoneAction>());

		// カメラ演出終了
		if (vCon_->GetRelease(BindActionType::kBoost)) {
			cameraEffector_->StartZoom(-boostCameraFov, boostCameraEffectTime * 2.0f);
		}
	}
}