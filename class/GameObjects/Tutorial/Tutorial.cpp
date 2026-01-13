#include "Tutorial.h"
#include "TutorialData/MoveTutorial.h"
#include "TutorialData/CollectTutorial.h"
#include "TutorialData/AttackTutorial.h"
#include "../Player/Player.h"

Tutorial::Tutorial(Player* player, EnemyManager* enemyManager) {
	player_ = player;
	enemyManager_ = enemyManager;

	guideData_ = std::make_unique<AttackTutorial>(player_, enemyManager_);

	sequence_ = GuideSequence::kMove;

	// 所持している武器をすべて破棄
	player_->GetWeaponController()->DeleteWeapons();
}

Tutorial::~Tutorial() {

}

void Tutorial::Init() {

}

void Tutorial::Update() {
	if (isFinish_) { return; }

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