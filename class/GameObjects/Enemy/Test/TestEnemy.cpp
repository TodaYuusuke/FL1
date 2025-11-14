#include "TestEnemy.h"
#include "../EnemyConfig.h"

using namespace LWP::Math;

TestEnemy::TestEnemy() {
	tag_ = EnemyConfig::tag;
	name_ = EnemyConfig::name + std::to_string(10000);
	attackPower_ = 1;

	// モデル生成
	model_.LoadFullPath(EnemyConfig::ModelName::modelName[(int)EnemyType::kTest]);

	velocity_ = { 0.1f,0.0f,0.0f };
}

TestEnemy::~TestEnemy() {

}

void TestEnemy::Init() {

}

void TestEnemy::Update() {
	// ヒットストップ
	HitStopController* stopController = HitStopController::GetInstance();

	model_.worldTF.translation += velocity_ * stopController->GetDeltaTime();

	// 速度に応じて角度変更
	if (Vector3::Dot(velocity_, velocity_) != 0.0f) model_.worldTF.rotation = Quaternion::LookRotation(velocity_);
}

void TestEnemy::DrawGui() {
	if (ImGui::TreeNode("TestEnemy")) {
		if (ImGui::TreeNode("Model")) {
			model_.DebugGUI();
			ImGui::TreePop();
		}
		ImGui::DragFloat3("Velocity", &velocity_.x, 0.01f);

		// 削除
		if (ImGui::Button("Delete")) {
			isAlive_ = false;
		}
		ImGui::TreePop();
	}
}
