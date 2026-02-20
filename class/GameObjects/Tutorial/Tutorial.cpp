#include "Tutorial.h"
#include "TutorialData/MoveTutorial.h"
#include "TutorialData/CollectTutorial.h"
#include "TutorialData/AttackTutorial.h"
#include "../Player/Player.h"

Tutorial::Tutorial(Player* player, EnemyManager* enemyManager) {
	player_ = player;
	enemyManager_ = enemyManager;

	guideData_ = std::make_unique<MoveTutorial>(player_, enemyManager_);

	sequence_ = GuideSequence::kMove;

	// 終了コマンド
	finishCommands_.push_back(BindActionType::kCancel);

	finishFrame_ = pressFisnishCommandFrame;

	finishSprites_["shadow"].LoadTexture("UI/Tutorial/finish_tutorial.png");
	finishSprites_["shadow"].anchorPoint = { 0.0f, 1.0f };
	finishSprites_["shadow"].worldTF.translation = { 100.0f,200.0f,0.0f };
	finishSprites_["shadow"].material.color.R = 0;
	finishSprites_["shadow"].material.color.G = 0;
	finishSprites_["shadow"].material.color.B = 255;
	finishSprites_["shadow"].material.color.A = 255;
	finishSprites_["shadow"].clipRect.min.y = finishSprites_["shadow"].material.texture.t.GetSize().y;
	finishSprites_["shadow"].clipRect.max = finishSprites_["shadow"].material.texture.t.GetSize();

	finishSprites_["body"].LoadTexture("UI/Tutorial/finish_tutorial.png");
	finishSprites_["body"].anchorPoint = { 0.0f, 1.0f };
	finishSprites_["body"].worldTF.translation = { 100.0f,200.0f,0.0f };
	finishSprites_["body"].material.color.A = 100;
	finishSprites_["body"].clipRect.max = finishSprites_["body"].material.texture.t.GetSize();

	// 所持している武器をすべて破棄
	player_->GetWeaponController()->DeleteWeapons();
}

Tutorial::~Tutorial() {

}

void Tutorial::Init() {

}

void Tutorial::Update() {
	if (isFinish_) { return; }

	// チュートリアル終了
	FinishCommand();

	guideData_->Update();

	switch (sequence_) {
	case Tutorial::GuideSequence::kMove:
		if (guideData_->GetIsFinish()) {
			guideData_ = std::move(std::make_unique<CollectTutorial>(player_, enemyManager_));
			sequence_ = GuideSequence::kCollect;
		}
		break;
	case Tutorial::GuideSequence::kCollect:
		if (guideData_->GetIsFinish()) {
			guideData_ = std::move(std::make_unique<AttackTutorial>(player_, enemyManager_));
			sequence_ = GuideSequence::kAttack;
		}
		break;
	case Tutorial::GuideSequence::kAttack:
		if (guideData_->GetIsFinish()) {
			sequence_ = GuideSequence::kCount;
		}
		break;
	case Tutorial::GuideSequence::kCount:
		isFinish_ = true;
		break;
	default:
		break;
	}
}

void Tutorial::FinishCommand() {
	// 終了コマンドが一つでも押されていないなら終了
	for (BindActionType key : finishCommands_) {
		if (!VirtualController::GetInstance()->GetPress(key)) { 
			finishFrame_ = pressFisnishCommandFrame;
			finishSprites_["shadow"].clipRect.min.y = finishSprites_["shadow"].material.texture.t.GetSize().y;
			return; 
		}
	}

	finishSprites_["shadow"].clipRect.min.y = (finishFrame_ / pressFisnishCommandFrame) * finishSprites_["shadow"].material.texture.t.GetSize().y;

	// 指定の時間押し続けたら終了
	if (finishFrame_ <= 0) {
		isFinish_ = true;
	}

	finishFrame_--;
}