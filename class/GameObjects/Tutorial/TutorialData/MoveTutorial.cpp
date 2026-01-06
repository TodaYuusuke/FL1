#include "BaseTutorialData.h"
#include "MoveTutorial.h"
#include "../../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;

MoveTutorial::MoveTutorial(Player* player, EnemyManager* enemyManager) {
	vCon_ = VirtualController::GetInstance();
	player_ = player;
	enemyManager_ = enemyManager;

	dir_ = Vector3{ 0,0,1 } *Matrix4x4::CreateRotateXYZMatrix(player->GetRot());
	centerPos_ = { LWP::Info::GetWindowWidthF() / 2.0f, 200.0f, 0.0f };

	guideFont_[MoveTutorial::MoveGuideSequence::kMoveF].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveF].LoadTexture("UI/Tutorial/forward_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveF].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kMoveB].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveB].LoadTexture("UI/Tutorial/back_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveB].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kMoveL].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveL].LoadTexture("UI/Tutorial/left_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveL].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kMoveR].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveR].LoadTexture("UI/Tutorial/right_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveR].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kCameraL].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kCameraL].LoadTexture("UI/Tutorial/cameraLeft_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kCameraL].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kCameraR].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kCameraR].LoadTexture("UI/Tutorial/cameraRight_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kCameraR].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kTurn].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kTurn].LoadTexture("UI/Tutorial/quickturn_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kTurn].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kBoost].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kBoost].LoadTexture("UI/Tutorial/boost_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kBoost].isActive = false;
}

void MoveTutorial::Init() {
	sequence_ = MoveTutorial::MoveGuideSequence::kMoveF;
	isFinish_ = false;
}

void MoveTutorial::Update() {
	for (auto& [key, sprite] : guideFont_) {
		sprite.worldTF.translation = centerPos_;

		// 
		if (key == sequence_) {
			sprite.isActive = true;
			continue;
		}
		sprite.isActive = false;
	}

	Vector2 lStick{};
	Vector2 rStick{};
	switch (sequence_) {
	case MoveTutorial::MoveGuideSequence::kMoveF:
		// スティック入力の補正
		lStick = AdjustmentStick(vCon_->GetLAxis());
		rStick = AdjustmentStick(vCon_->GetRAxis());
		if (lStick.y + rStick.y >= 2.0f) {
			sequence_ = MoveTutorial::MoveGuideSequence::kMoveB;
		}

		break;
	case MoveTutorial::MoveGuideSequence::kMoveB:
		// スティック入力の補正
		lStick = AdjustmentStick(vCon_->GetLAxis());
		rStick = AdjustmentStick(vCon_->GetRAxis());
		if (lStick.y + rStick.y <= -2.0f) {
			sequence_ = MoveTutorial::MoveGuideSequence::kMoveL;
		}

		break;
	case MoveTutorial::MoveGuideSequence::kMoveL:
		// スティック入力の補正
		lStick = AdjustmentStick(vCon_->GetLAxis());
		rStick = AdjustmentStick(vCon_->GetRAxis());
		if (lStick.x + rStick.x <= -2.0f) {
			sequence_ = MoveTutorial::MoveGuideSequence::kMoveR;
		}

		break;
	case MoveTutorial::MoveGuideSequence::kMoveR:
		// スティック入力の補正
		lStick = AdjustmentStick(vCon_->GetLAxis());
		rStick = AdjustmentStick(vCon_->GetRAxis());
		if (lStick.x + rStick.x >= 2.0f) {
			sequence_ = MoveTutorial::MoveGuideSequence::kCameraL;
		}

		break;
	case MoveTutorial::MoveGuideSequence::kCameraL:
		// スティック入力の補正
		lStick = AdjustmentStick(vCon_->GetLAxis());
		rStick = AdjustmentStick(vCon_->GetRAxis());
		if (lStick.y <= -1.0f || rStick.y >= 1.0f) {
			sequence_ = MoveTutorial::MoveGuideSequence::kCameraR;
		}

		break;
	case MoveTutorial::MoveGuideSequence::kCameraR:
		// スティック入力の補正
		lStick = AdjustmentStick(vCon_->GetLAxis());
		rStick = AdjustmentStick(vCon_->GetRAxis());
		if (lStick.y >= 1.0f || rStick.y <= -1.0f) {
			sequence_ = MoveTutorial::MoveGuideSequence::kTurn;
		}

		break;
	case MoveTutorial::MoveGuideSequence::kTurn:
		// 自機が回転した
		if (player_->GetMoveController()->GetIsTurnBehind()) {
			sequence_ = MoveTutorial::MoveGuideSequence::kBoost;
		}

		break;
	case MoveTutorial::MoveGuideSequence::kBoost:
		// 加速している
		if (vCon_->GetPress(BindActionType::kBoost)) {
			isFinish_ = true;
			sequence_ = MoveTutorial::MoveGuideSequence::kCount;
		}

		break;
	default:
		break;
	}
}

LWP::Math::Vector2 MoveTutorial::AdjustmentStick(LWP::Math::Vector2 stick) {
	Vector2 result = { 0.0f,0.0f };
	if (std::fabsf(stick.x) >= 0.8f) {
		if (std::signbit(stick.x)) { result.x = -1.0f; }
		else { result.x = 1.0f; }
	}
	result.x = stick.x;
	if (std::fabsf(stick.y) >= 0.01f) {
		if (std::signbit(stick.y)) { result.y = -1.0f; }
		else { result.y = 1.0f; }
	}

	return result;
}