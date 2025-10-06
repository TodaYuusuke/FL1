#include "Title.h"
#include "NullScene.h"
#include "GameScene.h"
#include "../GameObjects/Player/Player.h"
#include "../GameObjects/BehaviourTree/Actor/Enemy/Attacker.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

Title::~Title() {
	world_->Clear();
	delete world_;
	delete behaviorTree_;
}

void Title::Initialize() {	
	// カメラ位置調整
	mainCamera.worldTF.translation = { 0.0f,0.0f,-10.0f };

	behaviorTree_ = new BehaviorTreeGraph(true);
	behaviorTree_->SelectLoadFile("behavior_tree.json");

	world_ = new World();
	world_->AddActor(new Player(&mainCamera));
	//world_.AddActor(new Attacker(&world_, &mainCamera, ""));
}

void Title::Update() {
	world_->Update(1.0f);
}