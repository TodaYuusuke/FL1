#include "GameScene.h"
#include "NullScene.h"
#include "Title.h"
#include "ResultScene.h"
#include "../GameObjects/Camera/Effect/CameraEffectHandler.h"
#include "../GameObjects/Attack/AttackManager.h"
#include "../GameObjects/Weapon/WeaponManager.h"
#include "../GameObjects/UI/ScoreUI/ScoreManager.h"
#include "../GameObjects/UI/Radar/Radar.h"
#include "../GameObjects/PenetrationResolver/PenetrationResolver.h"
#include "../GameObjects/WaveManager.h"
#include "../Componets/HitStopController.h"
#include "../Componets/Input/VirtualController.h"
#include "../Componets/InputMyController/ControllerReceiver.h"
#include "../Effect/EffectManager.h"
#include "../Effect/EffectEditor.h"
#include "../Effect/Bullet/BulletEffector.h"

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
	// ウェーブ
	WaveManager::Destroy();
	// エフェクト
	EffectEditor::Destroy();
	EffectManager::Destroy();
	BulletEffector::Destroy();
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

	Radar::Destroy();

	//マイコン入力の停止
	ControllerReceiver::GetInstance()->ClosePort();
	// カメラ演出
	CameraEffectHandler::Destroy();

	// 効果音プレイヤー生成
	SEPlayer::Destroy();
}

void GameScene::Initialize() {
	// 効果音プレイヤー生成
	SEPlayer::Create();
	// エフェクト関連のインスタンス生成
	EffectManager::Create();
	EffectEditor::Create();
	BulletEffector::Create();

	// エフェクト関連初期化
	EffectManager::GetInstance()->Init();
	EffectEditor::GetInstance()->SetEffectManager(EffectManager::GetInstance());
	EffectEditor::GetInstance()->Init();

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

	Radar::Create();

	// 押し出し
	PenetrationResolver::Create();
	// ウェーブ
	WaveManager::Create();

	// 地形情報読み込み
	levelData.LoadShortPath("gameScene.json");

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>(&mainCamera);

	// 弾エフェクター用にカメラを設定
	BulletEffector::GetInstance()->Init(followCamera_->GetCamera());

	// 世界
	world_ = std::make_unique<World>();

	// 自機
	player_ = new Player(followCamera_.get(), world_.get(), followCamera_->defaultTargetDist_);
	// 自機をアクターとして追加
	world_->AddActor(player_);

	// 効果音プレイヤーに自機のトランスフォームを渡す
	SEPlayer::GetInstance()->SetListener(player_->GetWorldTF());

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

	//スコアを0に
	ScoreCounter::GetInstance()->Reset();

	
	Radar::GetInstance()->Initialize();
	Radar::GetInstance()->SetPlayerTransform(player_->GetWorldTF());
	Radar::GetInstance()->SetParent(player_->GetWeaponController()->GetCockpit());
	enemyManager_->SetMiniMapFunc(Radar::AppendTargetEnemy);
	WeaponManager::GetInstance()->SetMiniMapFunc(Radar::AppendTargetWeapon);
	world_->SetMiniMapFunc(Radar::AppendTargetProp);

	//シーン遷移アニメーション
	sceneChangeAnimation_ = std::make_unique<SceneChangeAnimationPlane>();
	sceneChangeAnimation_->Initialize();
	sceneChangeAnimation_->SetAnimationLength(animationLength_);
	//sceneChangeAnimation_->Start(0);
	sceneChangeAnimation_->SetParent(player_->GetWorldTF());
	isChangeScene_ = false;
	isEndStartAnimation_=false;

}

void GameScene::Update() {
	// 効果音プレイヤー生成
	SEPlayer::GetInstance()->Update();

	// 敵を一定数倒したら終了
	if (!player_->GetIsAlive()) {
		ChangeResultScene();
	}
	// ウェーブ
	WaveManager::GetInstance()->Update();

	// ヒットストップ
	HitStopController::GetInstance()->Update();

	Radar::GetInstance()->ClearVector();

	// 敵管理
	enemyManager_->Update();

	// ワールドオブジェクト
	world_->Update();

	// 武器管理クラス
	WeaponManager::GetInstance()->Update();
	// 弾管理クラス
	AttackManager::GetInstance()->Update();

	// 追従カメラ
	followCamera_->Update();

	//スコア表示(テスト)
	ScoreCounter::GetInstance()->Update();
	score_->SetScore(ScoreCounter::GetInstance()->GetDrawScore());
	score_->Update();

	//ミニマップ
	Radar::GetInstance()->Update();

	//シーン遷移アニメーション
	if (player_->GetWeaponController()->GetIsEndAnimation() && !isEndStartAnimation_) {
		isEndStartAnimation_ = true;
		sceneChangeAnimation_->Start(0);
	}
	sceneChangeAnimation_->Update();

	// エフェクト関連更新
	EffectManager::GetInstance()->Update();
	BulletEffector::GetInstance()->Update();

	// カメラ演出
	CameraEffectHandler::GetInstance()->Update();

	// 押し出し
	PenetrationResolver::GetInstance()->Update();

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
	// ウェーブ
	WaveManager::GetInstance()->DebugGui();
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

	Radar::GetInstance()->DebugGui();

	ControllerReceiver::GetInstance()->DebugGUI();

	static bool is;
	// シーン遷移アニメーション
	if (LWP::Input::Keyboard::GetTrigger(DIK_7)) {
		sceneChangeAnimation_->Start(int(is));
		is = !is;
	}
	sceneChangeAnimation_->DebugGui();

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
	world_->EndFrame();

	// コントローラー
	VirtualController::GetInstance()->Update();
}

void GameScene::ChangeResultScene() {
	if (!isChangeScene_) sceneChangeAnimation_->Start(1);
	if (!sceneChangeAnimation_->GetIsPlay()) nextSceneFunction = []() { return new ResultScene(); };
	isChangeScene_ = true;
}