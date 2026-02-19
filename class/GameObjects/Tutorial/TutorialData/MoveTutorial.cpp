#include "BaseTutorialData.h"
#include "MoveTutorial.h"
#include "../../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;

MoveTutorial::MoveTutorial(Player* player, EnemyManager* enemyManager) {
	vCon_ = VirtualController::GetInstance();
	player_ = player;
	enemyManager_ = enemyManager;

	dir_ = Vector3{ 0,0,1 } *Matrix4x4::CreateRotateXYZMatrix(player->GetRot());
	centerPos_ = { LWP::Info::GetWindowWidthF() / 2.0f, 200.0f, 0.0f };

	// 開始スプライト
	introFont_.LoadTexture("UI/Tutorial/move_tutorial.png");
	introFont_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f, 200.0f, 0.0f };
	introFont_.material.color.A = 0;
	introFont_.isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kMoveF].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveF].LoadTexture("UI/Tutorial/forward_tutorial.png");
	echoFont_[MoveTutorial::MoveGuideSequence::kMoveF].LoadTexture("UI/Tutorial/forward_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveF].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kMoveB].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveB].LoadTexture("UI/Tutorial/back_tutorial.png");
	echoFont_[MoveTutorial::MoveGuideSequence::kMoveB].LoadTexture("UI/Tutorial/back_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveB].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kMoveL].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveL].LoadTexture("UI/Tutorial/left_tutorial.png");
	echoFont_[MoveTutorial::MoveGuideSequence::kMoveL].LoadTexture("UI/Tutorial/left_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveL].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kMoveR].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveR].LoadTexture("UI/Tutorial/right_tutorial.png");
	echoFont_[MoveTutorial::MoveGuideSequence::kMoveR].LoadTexture("UI/Tutorial/right_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kMoveR].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kCameraL].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kCameraL].LoadTexture("UI/Tutorial/cameraLeft_tutorial.png");
	echoFont_[MoveTutorial::MoveGuideSequence::kCameraL].LoadTexture("UI/Tutorial/cameraLeft_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kCameraL].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kCameraR].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kCameraR].LoadTexture("UI/Tutorial/cameraRight_tutorial.png");
	echoFont_[MoveTutorial::MoveGuideSequence::kCameraR].LoadTexture("UI/Tutorial/cameraRight_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kCameraR].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kTurn].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kTurn].LoadTexture("UI/Tutorial/quickturn_tutorial.png");
	echoFont_[MoveTutorial::MoveGuideSequence::kTurn].LoadTexture("UI/Tutorial/quickturn_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kTurn].isActive = false;

	guideFont_[MoveTutorial::MoveGuideSequence::kBoost].anchorPoint = { 0.5f, 0.5f };
	guideFont_[MoveTutorial::MoveGuideSequence::kBoost].LoadTexture("UI/Tutorial/boost_tutorial.png");
	echoFont_[MoveTutorial::MoveGuideSequence::kBoost].LoadTexture("UI/Tutorial/boost_tutorial.png");
	guideFont_[MoveTutorial::MoveGuideSequence::kBoost].isActive = false;

	for (auto& [key, sprite] : guideFont_) {
		echoFont_[key] = sprite;
	}

	// 連番画像
	serialAnim_ = std::make_unique<SerialAnimation>("UI/Tutorial/manual/forward_manual.png", Vector2{ 700.0f,256.0f });
	serialAnim_->SetPos({ LWP::Info::GetWindowWidthF() / 2.0f, 350.0f, 0.0f });

	//sequence_ = MoveTutorial::MoveGuideSequence::kBoost;
}

void MoveTutorial::Init() {
	sequence_ = MoveTutorial::MoveGuideSequence::kMoveF;
	isFinish_ = false;
}

void MoveTutorial::Update() {
	for (auto& [key, sprite] : guideFont_) {
		sprite.worldTF.translation = centerPos_;
		echoFont_[key].worldTF.translation = centerPos_;

		// 該当のチュートリアル以外画像非表示
		if (key == sequence_) {
			sprite.isActive = true;
			continue;
		}
		sprite.isActive = false;
	}

	Vector2 lStick{};
	Vector2 rStick{};
	switch (sequence_) {
	case MoveTutorial::MoveGuideSequence::kIntro:
		// 開始演出
		StartEffect();

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(0.0f)) {
			sequence_ = MoveTutorial::MoveGuideSequence::kMoveF;
			isNextGuide_ = false;
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case MoveTutorial::MoveGuideSequence::kMoveF:
		// スティック入力の補正
		lStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetLAxis());
		rStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetRAxis());
		if (lStick.y + rStick.y >= 2.0f) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if(isNextGuide_){ SuccessEffect(MoveTutorial::MoveGuideSequence::kMoveF); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) { 
			sequence_ = MoveTutorial::MoveGuideSequence::kMoveB;
			isNextGuide_ = false;
			serialAnim_->ChangeSprite("UI/Tutorial/manual/back_manual.png", Vector2{ 700.0f,256.0f });
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case MoveTutorial::MoveGuideSequence::kMoveB:
		// スティック入力の補正
		lStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetLAxis());
		rStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetRAxis());
		if (lStick.y + rStick.y <= -2.0f) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(MoveTutorial::MoveGuideSequence::kMoveB); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = MoveTutorial::MoveGuideSequence::kMoveL;
			isNextGuide_ = false;
			serialAnim_->ChangeSprite("UI/Tutorial/manual/leftMove_manual.png", Vector2{ 700.0f,256.0f });
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case MoveTutorial::MoveGuideSequence::kMoveL:
		// スティック入力の補正
		lStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetLAxis());
		rStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetRAxis());
		if (lStick.x + rStick.x <= -2.0f) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(MoveTutorial::MoveGuideSequence::kMoveL); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = MoveTutorial::MoveGuideSequence::kMoveR;
			isNextGuide_ = false;
			serialAnim_->ChangeSprite("UI/Tutorial/manual/rightMove_manual.png", Vector2{ 700.0f,256.0f });
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case MoveTutorial::MoveGuideSequence::kMoveR:
		// スティック入力の補正
		lStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetLAxis());
		rStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetRAxis());
		if (lStick.x + rStick.x >= 2.0f) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(MoveTutorial::MoveGuideSequence::kMoveR); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = MoveTutorial::MoveGuideSequence::kCameraL;
			isNextGuide_ = false;
			serialAnim_->ChangeSprite("UI/Tutorial/manual/leftTurn_manual.png", Vector2{ 700.0f,256.0f });
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case MoveTutorial::MoveGuideSequence::kCameraL:
		// スティック入力の補正
		lStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetLAxis());
		rStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetRAxis());
		if (lStick.y <= -1.0f && rStick.y >= 1.0f) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(MoveTutorial::MoveGuideSequence::kCameraL); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = MoveTutorial::MoveGuideSequence::kCameraR;
			isNextGuide_ = false;
			serialAnim_->ChangeSprite("UI/Tutorial/manual/rightTurn_manual.png", Vector2{ 700.0f,256.0f });
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case MoveTutorial::MoveGuideSequence::kCameraR:
		// スティック入力の補正
		lStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetLAxis());
		rStick = player_->GetMoveController()->GetMove()->AdjustmentStick(vCon_->GetRAxis());
		if (lStick.y >= 1.0f && rStick.y <= -1.0f) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(MoveTutorial::MoveGuideSequence::kCameraR); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = MoveTutorial::MoveGuideSequence::kTurn;
			isNextGuide_ = false;
			serialAnim_->ChangeSprite("UI/Tutorial/manual/quickTurn_manual.png", Vector2{ 700.0f,256.0f });
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case MoveTutorial::MoveGuideSequence::kTurn:
		// 自機が回転した
		if (player_->GetMoveController()->GetIsTurnBehind()) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(MoveTutorial::MoveGuideSequence::kTurn); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = MoveTutorial::MoveGuideSequence::kBoost;
			isNextGuide_ = false;
			serialAnim_->ChangeSprite("UI/Tutorial/manual/boost_manual.png", Vector2{ 700.0f,256.0f });
			serialAnim_->Start(10.0f, 30.0f, true);
		}

		break;
	case MoveTutorial::MoveGuideSequence::kBoost:
		// 加速している
		if (vCon_->GetPress(BindActionType::kBoost)) {
			isNextGuide_ = true;
			serialAnim_->Init();
		}

		// 成功演出
		if (isNextGuide_) { SuccessEffect(MoveTutorial::MoveGuideSequence::kBoost); }

		// 条件を満たしたら次の説明に遷移
		if (NextGuide(60.0f)) {
			sequence_ = MoveTutorial::MoveGuideSequence::kCount;
			serialAnim_->Init();
		}

		break;
	default:
		// 終了演出
		FinishEffect();

		break;
	}

	serialAnim_->Update();
}

void MoveTutorial::SuccessEffect(MoveGuideSequence target) {
	const float endFrame = 30.0f;
	if (nextGuideInterval_ >= endFrame) {
		echoFont_[target].isActive = false;
		return;
	}

	// チュートリアル成功時効果音を鳴らす
	if (!echoFont_[target].isActive) { AudioPlayer::GetInstance()->PlayAudio("Success_SE.mp3", 1.0f, LWP::AudioConfig::Other); }

	// 衝撃波
	echoFont_[target].isActive = true;
	echoFont_[target].worldTF.scale = LWP::Utility::Interp::Lerp(Vector3{ 1,1,1 }, Vector3{ 2,2,2 }, Easing::OutExpo(nextGuideInterval_ / endFrame));
	echoFont_[target].material.color.A = static_cast<int>(LWP::Utility::Interp::LerpF(200.0f, 0.0f, Easing::OutExpo(nextGuideInterval_ / endFrame)));
}