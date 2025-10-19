#include "Title.h"
#include "../GameObjects/Enemy/Melee/MeleeAttacker.h"
#include "../GameObjects/Bullets/BulletManager.h"

using namespace LWP;
using namespace LWP::Math;

Title::Title() {
	// 弾管理クラスを生成
	BulletManager::Create();
}

Title::~Title() {
	BulletManager::Destroy();
}

// 初期化
void Title::Initialize() {
	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(&mainCamera);
	// 世界
	world_ = std::make_unique<World>();
	// 自機
	player_ = new Player(followCamera_->GetCamera());
	// 敵管理クラス
	enemyManager_ = std::make_unique<EnemyManager>(world_.get());

	// 地形情報読み込み
	levelData.LoadShortPath("gameScene.json");

	// 自機をアクターとして追加
	world_->AddActor(player_);
	// 敵管理リストを設定
	player_->SetEnemyManager(enemyManager_.get());
	// 追従カメラを自機対象に設定
	followCamera_->SetTarget(player_);
}

// 更新
void Title::Update() {
	world_->Update();
	// 敵管理
	enemyManager_->Update();

	// 弾の管理
	BulletManager::GetInstance()->Update();

	// 追従カメラ
	followCamera_->Update();

#ifdef _DEBUG
	world_->DebugGui();

	// 敵管理
	enemyManager_->DebugGui();

	// カメラ
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