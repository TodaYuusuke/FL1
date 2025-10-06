#include "Title.h"
#include "TestScene.h"
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/BehaviourTree/Actor/Enemy/Attacker.h"

using namespace LWP;

Title::~Title() {

}

// 初期化
void Title::Initialize() {
	// カメラ位置調整
	mainCamera.worldTF.translation = { 0.0f,0.0f,-10.0f };

	behaviorTree_ = std::make_unique<BehaviorTreeGraph>(true);
	behaviorTree_->SelectLoadFile("resources/json/behavior_tree.json");

	world_ = std::make_unique<World>();
	world_->AddActor(new Player(&mainCamera));
	world_->AddActor(new Attacker(world_.get(), &mainCamera, "resources/json/behavior_tree.json"));
}

// 更新
void Title::Update() {
	world_->Update(1.0f);

#ifdef _DEBUG
	world_->DebugGui();

	behaviorTree_->Update();
	behaviorTree_->Draw();
#endif // DEBUG
}