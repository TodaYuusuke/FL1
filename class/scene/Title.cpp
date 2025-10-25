#include "Title.h"
#include "../GameObjects/Enemy/Melee/MeleeAttacker.h"
#include "../GameObjects/Bullets/BulletManager.h"
#include "../GameObjects/Weapon/WeaponManager.h"

using namespace LWP;
using namespace LWP::Math;

Title::Title() {
	// 弾管理クラスを生成
	BulletManager::Create();
	WeaponManager::Create();
}

Title::~Title() {
	WeaponManager::Destroy();
	BulletManager::Destroy();
}

// 初期化
void Title::Initialize() {
	// 地形情報読み込み
	levelData.LoadShortPath("gameScene.json");

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(&mainCamera);

	// 世界
	world_ = std::make_unique<World>();

	// 自機
	player_ = std::make_unique<Player>(followCamera_->GetCamera());
	// 自機をアクターとして追加
	world_->AddActor(player_.get());

	// 敵管理クラス
	enemyManager_ = std::make_unique<EnemyManager>(world_.get());

	// 敵管理リストを設定
	player_->SetEnemyManager(enemyManager_.get());
	// 追従カメラを自機対象に設定
	followCamera_->SetTarget(player_.get());

	// 武器管理クラスに自機のアドレスを登録
	WeaponManager::GetInstance()->SetPlayer(player_.get());
	// 武器管理クラスにワールドのアドレスを登録
	WeaponManager::GetInstance()->SetWorld(world_.get());
}

// 更新
void Title::Update() {
	world_->Update();
	// 敵管理
	enemyManager_->Update();

	// 弾管理クラス
	BulletManager::GetInstance()->Update();
	// 武器管理クラス
	WeaponManager::GetInstance()->Update();

	// 追従カメラ
	followCamera_->Update();

#ifdef _DEBUG
	ImGui::Begin("GameObjects");
	ImGui::BeginTabBar("GameObject");

	world_->DebugGui();

	// 武器管理クラス
	WeaponManager::GetInstance()->DebugGui();
	// 敵管理
	enemyManager_->DebugGui();

	// カメラ
	if (ImGui::BeginTabItem("Camera")) {
		// 追従カメラ
		if (ImGui::TreeNode("Follow")) {
			followCamera_->DebugGUI();
			// 追従対象を設定
			if (ImGui::Button("SetFollow")) {
				followCamera_->SetTarget(player_.get());
			}
			// 追従をやめる
			if (ImGui::Button("ResetFollow")) {
				followCamera_->ResetTarget();
			}
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
	ImGui::End();
#endif // DEBUG
}