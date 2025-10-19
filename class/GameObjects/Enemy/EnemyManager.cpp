#include "EnemyManager.h"
#include "Melee/MeleeAttacker.h"
#include "Test/TestEnemy.h"
#include "EnemyConfig.h"

EnemyManager::EnemyManager(IWorld* world) {
	pWorld_ = world;
}

EnemyManager::~EnemyManager() {
	enemies_.clear();
}

void EnemyManager::Init() {
	for (Actor* actor : enemies_) {
		actor->Init();
	}
}

void EnemyManager::Update() {
	for (Actor* actor : enemies_) {
		// 前回の座標
		actor->SetPreTranslation(actor->GetWorldTF()->GetWorldPosition());

		actor->Update();
	}

	// 削除
	for (Actor* actor : enemies_) {
		if (actor->GetIsAlive()) continue;

		delete actor;
		auto newEnd = std::remove(enemies_.begin(), enemies_.end(), actor);
		enemies_.erase(newEnd, enemies_.end());
	}
}

void EnemyManager::DebugGui() {
	if (ImGui::TreeNode("Enemy")) {
		if (ImGui::TreeNode("Create")) {
			// 生成座標
			ImGui::DragFloat3("Translation", &createPos_.x, 0.01f);
			// 速度
			ImGui::DragFloat3("Velocity", &createVel_.x, 0.01f);
			// 敵生成
			if (ImGui::Button("Done")) {
				CreateTestEnemy();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void EnemyManager::CreateMeleeEnemy() {
	// 近接敵
	MeleeAttacker* actor = new MeleeAttacker(pWorld_, createID_, "resources/json/Attacker_tree.json");
	actor->SetTranslation(createPos_);
	enemies_.push_back(actor);

	createID_++;
}

void EnemyManager::CreateTestEnemy() {
	// テスト敵
	TestEnemy* actor = new TestEnemy();
	actor->SetTranslation(createPos_);
	actor->SetVelocity(createVel_);
	enemies_.push_back(actor);
}
