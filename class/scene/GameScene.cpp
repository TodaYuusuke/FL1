#include "GameScene.h"
#include "NullScene.h"
#include "Title.h"
#include "../GameObjects/Bullets/BulletManager.h"
#include "../GameObjects/Weapon/WeaponManager.h"
#include "../Componets/HitStopController.h"
#include "../GameObjects/UI/ScoreUI/ScoreManager.h"
#include "ResultScene.h"
#include "../Componets/InputMyController/ControllerReceiver.h"


using namespace LWP;
using namespace LWP::Resource;
using namespace LWP::Input;
using namespace LWP::Object;
using namespace LWP::Math;
using namespace LWP::Primitive;
using namespace LWP::Utility;
using namespace LWP::Info;

GameScene::GameScene() {
	// ヒットストップクラス
	HitStopController::Create();
	// 弾管理クラスを生成
	BulletManager::Create();
	// 武器管理クラスを作成
	WeaponManager::Create();
}

GameScene::~GameScene() {
	// 武器管理クラス
	BulletManager::Destroy();
	// 弾管理クラス
	WeaponManager::Destroy();
	// ヒットストップクラス
	HitStopController::Destroy();

	//マイコン入力の停止
	ControllerReceiver::GetInstance()->ClosePort();
}

void GameScene::Initialize() {
	// 地形情報読み込み
	levelData.LoadShortPath("gameScene.json");

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(&mainCamera);

	// 世界
	world_ = std::make_unique<World>();

	// 自機
	Player* player = new Player(followCamera_->GetCamera(), followCamera_->defaultTargetDist_);
	// 自機をアクターとして追加
	world_->AddActor(player);

	// 敵管理クラス
	enemyManager_ = std::make_unique<EnemyManager>(world_.get());

	// 敵管理リストを設定
	player->SetEnemyManager(enemyManager_.get());
	// 追従カメラを自機対象に設定
	followCamera_->SetTarget(player);

	// 武器管理クラスに自機のアドレスを登録
	WeaponManager::GetInstance()->SetPlayer(player);
	// 武器管理クラスにワールドのアドレスを登録
	WeaponManager::GetInstance()->SetWorld(world_.get());

	//スコア表示テスト
	score_ = std::make_unique<ScoreUI>();
	score_->Initialize(7);
	score_->SetCenter({1280.0f,100.0f});
}

void GameScene::Update() {
	// ヒットストップ
	HitStopController::GetInstance()->Update();

	// ワールドオブジェクト
	world_->Update();
	// 敵管理
	enemyManager_->Update();
	// 弾管理クラス
	BulletManager::GetInstance()->Update();
	// 武器管理クラス
	WeaponManager::GetInstance()->Update();

	// 追従カメラ
	followCamera_->Update();

	//スコア表示(テスト)
	score_->SetScore(ScoreCounter::GetInstance()->GetScore());
	score_->Update();


#ifdef _DEBUG

	// 次のシーンへ以降
	if (Input::Keyboard::GetTrigger(DIK_F)) {
		nextSceneFunction = []() { return new ResultScene(); };
	}

	ImGui::Begin("GameObjects");
	ImGui::BeginTabBar("GameObject");

	// ワールド
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
				followCamera_->SetTarget(world_->FindActor("Player"));
			}
			// 追従をやめる
			if (ImGui::Button("ResetFollow")) {
				followCamera_->ResetTarget();
			}
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}

	// ヒットストップ
	HitStopController::GetInstance()->DebugGui();

	ImGui::EndTabBar();
	ImGui::End();
#endif // DEBUG
}
