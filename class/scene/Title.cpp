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

	// カメラ位置調整
	mainCamera.worldTF.translation = { 0.0f,200.0f,0.0f };
	// 角度[ラジアン]
	Vector3 rot = {
		3.14f / 2.0f,
		0.0f,
		0.0f,
	};
	// 角度代入
	mainCamera.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 1,0,0 }, rot.x);
	mainCamera.worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, rot.y) * mainCamera.worldTF.rotation;

	behaviorTree_ = std::make_unique<BehaviorTreeGraph>(true);
	behaviorTree_->SelectLoadFile("resources/json/behavior_tree.json");

	// 自機
	player_ = new Player(followCamera_->GetCamera());

	// 世界
	world_ = std::make_unique<World>();
	world_->AddActor(player_);
	world_->AddActor(new Attacker(world_.get(), &mainCamera, "resources/json/behavior_tree.json"));

	// 追従対象の設定
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
		// デバッグ用カメラ
		if (ImGui::TreeNode("Debug")) {
			mainCamera.DebugGUI();
			ImGui::TreePop();
		}
		// 追従カメラ
		if (ImGui::TreeNode("Follow")) {
			followCamera_->DebugGUI();

			if (ImGui::Button("SetFollow")) {
				followCamera_->SetTarget(player_);
			}
			if (ImGui::Button("ResetFollow")) {
				followCamera_->ResetTarget();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif // DEBUG
}