#include "Title.h"
#include "TestScene.h"
#include "../GameObjects/BehaviourTree/Actor/Enemy/Attacker.h"
#include "../GameObjects/Bullets/BulletManager.h"

using namespace LWP;
using namespace LWP::Math;

Title::Title() {
	// 弾管理クラスを生成
	BulletManager::Create();
}

Title::~Title() {
	BulletManager::Destroy();
	//delete player_;
}

// 初期化
void Title::Initialize() {
	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(&mainCamera);

	// 世界
	world_ = std::make_unique<World>();
	world_->AddActor(player_);
	world_->AddActor(new Attacker(world_.get(), followCamera_->GetCamera(), "resources/json/behavior_tree.json"));

	// ビヘイビアツリー(試運転)
	behaviorTree_ = std::make_unique<BehaviorTreeGraph>(true);
	behaviorTree_->SelectLoadFile("resources/json/behavior_tree.json");

	// 自機
	player_ = new Player(followCamera_->GetCamera());

	// 追従カメラの対象設定
	followCamera_->SetTarget(player_);

	// 地形情報読み込み
	levelData.LoadShortPath("gameScene.json");
}

// 更新
void Title::Update() {
	world_->Update();

	BulletManager::GetInstance()->Update();

	// 追従カメラ
	followCamera_->Update();

#ifdef _DEBUG
	world_->DebugGui();

	behaviorTree_->Update();
	behaviorTree_->Draw();

	if (ImGui::TreeNode("Camera")) {
		// 追従カメラ
		if (ImGui::TreeNode("Follow")) {
			followCamera_->DebugGUI();
			// 追従対象を設定
			if (ImGui::Button("SetFollow")) {
				followCamera_->SetTarget(player_);
			}
			// 追従をやめる
			if (ImGui::Button("ResetFollow")) {
				followCamera_->ResetTarget();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif // DEBUG
}