#include "Tutorial.h"
#include "TutorialData/MoveTutorial.h"

Tutorial::Tutorial(Player* player, EnemyManager* enemyManager) {
	player_ = player;
	enemyManager_ = enemyManager;

	guideData_.push_back(new MoveTutorial(player, enemyManager_));
}

Tutorial::~Tutorial() {
	for (BaseTutorialData* data : guideData_) {
		delete data;
		data = nullptr;
	}
}

void Tutorial::Init() {

}

void Tutorial::Update() {
	if (isFinish_) { return; }

	guideData_[currentGuideNum_]->Update();

	if (guideData_[currentGuideNum_]->GetIsFinish()) {
		if (currentGuideNum_ < (int)guideData_.size() - 1) {
			currentGuideNum_++;
		}
		else {
			isFinish_ = true;
		}
	}
}