#include "GameScene.h"
#include "NullScene.h"
#include "Title.h"
#include "ResultScene.h"
#include "../GameObjects/Camera/Effect/CameraEffectHandler.h"
#include "../GameObjects/Attack/AttackManager.h"
#include "../GameObjects/Weapon/WeaponManager.h"
#include "../GameObjects/UI/ScoreUI/ScoreManager.h"
#include "../GameObjects/PenetrationResolver/PenetrationResolver.h"
#include "../Componets/HitStopController.h"
#include "../Componets/Input/VirtualController.h"
#include "../Componets/InputMyController/ControllerReceiver.h"
#include "../Effect/EffectManager.h"
#include "../Effect/EffectEditor.h"

using namespace LWP;
using namespace LWP::Resource;
using namespace LWP::Input;
using namespace LWP::Object;
using namespace LWP::Math;
using namespace LWP::Primitive;
using namespace LWP::Utility;
using namespace LWP::Info;

GameScene::GameScene() {
}

GameScene::~GameScene() {
	// 
	EffectEditor::Destroy();
	EffectManager::Destroy();
	// 押し出し
	PenetrationResolver::Destroy();
	// 武器管理クラス
	AttackManager::Destroy();
	// 弾管理クラス
	WeaponManager::Destroy();
	// ヒットストップクラス
	HitStopController::Destroy();
	// ゲームコントローラ
	VirtualController::Destroy();
	//マイコン入力の停止
	ControllerReceiver::GetInstance()->ClosePort();
	// カメラ演出
	CameraEffectHandler::Destroy();
}

void GameScene::Initialize() {
	// カメラ演出
	CameraEffectHandler::Create();
	//ポート再オープン
	ControllerReceiver::GetInstance()->ReOpenPort();
	// ゲームコントローラ
	VirtualController::Create();
	// ヒットストップクラス
	HitStopController::Create();
	// 弾管理クラスを生成
	AttackManager::Create();
	// 武器管理クラスを作成
	WeaponManager::Create();
	// 押し出し
	PenetrationResolver::Create();
	// インスタンス生成
	EffectManager::Create();
	EffectEditor::Create();

	// 地形情報読み込み
	levelData.LoadShortPath("gameScene.json");

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(&mainCamera);

	// 世界
	world_ = std::make_unique<World>();

	// 自機
	player_ = new Player(followCamera_.get(), world_.get(), followCamera_->defaultTargetDist_);
	// 自機をアクターとして追加
	world_->AddActor(player_);

	// 敵管理クラス
	enemyManager_ = std::make_unique<EnemyManager>(world_.get());

	// 敵管理リストを設定
	player_->SetEnemyManager(enemyManager_.get());
	// 追従カメラを自機対象に設定
	followCamera_->SetTarget(player_);

	// 武器管理クラスに自機のアドレスを登録
	WeaponManager::GetInstance()->SetPlayer(player_);
	// 武器管理クラスにワールドのアドレスを登録
	WeaponManager::GetInstance()->SetWorld(world_.get());

	PenetrationResolver::GetInstance()->RegisterObject(player_, 1);

	// 調整項目
	json_.Init("Game.json")
		.AddValue<int>("ClearKillCount", &clearKillCount)
		.CheckJsonFile();

	//スコア表示テスト
	score_ = std::make_unique<ScoreUI>();
	score_->Initialize(7);
	score_->SetCenter({1280.0f,100.0f});

	// 演出対象のカメラ
	CameraEffectHandler::GetInstance()->SetEffectTarget(followCamera_.get());

	// エフェクト関連初期化
	EffectManager::GetInstance()->Init();
	EffectEditor::GetInstance()->SetEffectManager(EffectManager::GetInstance());
	EffectEditor::GetInstance()->Init();

	//スコアを0に
	ScoreCounter::GetInstance()->Reset();
}

void GameScene::Update() {
	// 敵を一定数倒したら終了
	if (enemyManager_->GetKillCount() >= clearKillCount || !player_->GetIsAlive()) {
		nextSceneFunction = []() { return new ResultScene(); };
	}
	// ヒットストップ
	HitStopController::GetInstance()->Update();

	// 押し出し
	PenetrationResolver::GetInstance()->Update();

	// 敵管理
	enemyManager_->Update();

	// ワールドオブジェクト
	world_->Update();

	// 弾管理クラス
	AttackManager::GetInstance()->Update();
	// 武器管理クラス
	WeaponManager::GetInstance()->Update();

	// 追従カメラ
	followCamera_->Update();

	//スコア表示(テスト)
	score_->SetScore(ScoreCounter::GetInstance()->GetScore());
	score_->Update();

	// エフェクト関連初期化
	EffectManager::GetInstance()->Update();

	// カメラ演出
	CameraEffectHandler::GetInstance()->Update();

#ifdef _DEBUG

	// 次のシーンへ以降
	if (Input::Keyboard::GetTrigger(DIK_F)) {
		nextSceneFunction = []() { return new ResultScene(); };
	}

	//マイコンの再接続
	if (Input::Keyboard::GetTrigger(DIK_R)) {
		ControllerReceiver::GetInstance()->ReOpenPort();
	}

	// 演出のエディタ
	EffectEditor::GetInstance()->Update();

	ImGui::Begin("GameObjects");
	ImGui::BeginTabBar("GameObject");

	// ゲーム自体の設定
	if (ImGui::BeginTabItem("GameRule")) {
		json_.DebugGUI();
		ImGui::EndTabItem();
	}

	// ワールド
	world_->DebugGui();

	// 武器管理クラス
	WeaponManager::GetInstance()->DebugGui();
	// 弾管理クラス
	AttackManager::GetInstance()->DebugGui();
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
		if (ImGui::Button("SampleShake")) {
			CameraEffectHandler::GetInstance()->StartShake(Vector3{ 0.01f,0.01f,0.01f }, 1.0f);
		}
		if (ImGui::Button("SampleZoom")) {
			CameraEffectHandler::GetInstance()->StartZoom(10.0f, 0.5f);
		}
		if (ImGui::Button("SampleBoundMove")) {
			CameraEffectHandler::GetInstance()->StartBound(Vector3{ 0.0f,-0.5f,0.0f }, 0.5f);
		}
		ImGui::EndTabItem();
	}

	// ヒットストップ
	HitStopController::GetInstance()->DebugGui();

	ImGui::EndTabBar();
	ImGui::End();


	ImGui::Begin("LeftStick");
	//ImGui::Text("input  X: %.3f , Y: %.3f", ControllerReceiver::GetInstance()->GetData().GetRawData().x, microController_.GetRawData().y);
	ImGui::Text("calced X: %.3f , Y: %.3f", ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickLeft.lever.x, ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickLeft.lever.y);
	ImGui::Text("button0 %d", (int)ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickLeft.button0);
	ImGui::Text("button1 %d", (int)ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickLeft.button1);
	ImGui::End();

	ImGui::Begin("RightStick");
	//ImGui::Text("input  X: %.3f , Y: %.3f", ControllerReceiver::GetInstance()->GetData().GetRawData().x, microController_.GetRawData().y);
	ImGui::Text("calced X: %.3f , Y: %.3f", ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickRight.lever.x, ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickRight.lever.y);
	ImGui::Text("button0 %d", (int)ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickRight.button0);
	ImGui::Text("button1 %d", (int)ControllerReceiver::GetInstance()->GetData().stick.multiSticks.stickRight.button1);
	ImGui::End();

#endif // DEBUG

	// 更新処理終了時に呼ぶ処理
	enemyManager_->EndFrame();
	AttackManager::GetInstance()->EndFrame();

	// コントローラー
	VirtualController::GetInstance()->Update();
}
