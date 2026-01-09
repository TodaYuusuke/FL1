#include "TutorialScene.h"
#include "NullScene.h"
#include "Title.h"
#include "ResultScene.h"
#include "GameScene.h"
#include "../GameObjects/Camera/Effect/CameraEffectHandler.h"
#include "../GameObjects/Attack/AttackManager.h"
#include "../GameObjects/Weapon/WeaponManager.h"
#include "../GameObjects/UI/ScoreUI/ScoreManager.h"
#include "../GameObjects/PenetrationResolver/PenetrationResolver.h"
#include "../GameObjects/WaveManager.h"
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

TutorialScene::TutorialScene() {
}

TutorialScene::~TutorialScene() {
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

void TutorialScene::Initialize() {
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
	Player* player_ = new Player(followCamera_.get(), world_.get(), followCamera_->defaultTargetDist_);
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

	// 演出対象のカメラ
	CameraEffectHandler::GetInstance()->SetEffectTarget(followCamera_.get());

	// エフェクト関連初期化
	EffectManager::GetInstance()->Init();
	EffectEditor::GetInstance()->SetEffectManager(EffectManager::GetInstance());
	EffectEditor::GetInstance()->Init();

	// チュートリアル
	tutorial_ = std::make_unique<Tutorial>(player_, enemyManager_.get());

	easingEditor_ = std::make_unique<EasingEditor>();
}

void TutorialScene::Update() {
	// 敵を一定数倒したら終了
	if (tutorial_->GetIsFinish()) {
		nextSceneFunction = []() { return new GameScene(); };
	}

	// ヒットストップ
	HitStopController::GetInstance()->Update();

	// 押し出し
	PenetrationResolver::GetInstance()->Update();

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

	// エフェクト関連初期化
	EffectManager::GetInstance()->Update();

	// カメラ演出
	CameraEffectHandler::GetInstance()->Update();

	// チュートリアル
	tutorial_->Update();

#ifdef _DEBUG

	// 次のシーンへ以降
	if (Input::Keyboard::GetTrigger(DIK_F)) {
		//nextSceneFunction = []() { return new ResultScene(); };
	}

	//マイコンの再接続
	if (Input::Keyboard::GetTrigger(DIK_R)) {
		ControllerReceiver::GetInstance()->ReOpenPort();
	}

	easingEditor_->Update();

#endif // DEBUG

	// 更新処理終了時に呼ぶ処理
	enemyManager_->EndFrame();
	AttackManager::GetInstance()->EndFrame();

	// コントローラー
	VirtualController::GetInstance()->Update();
}
