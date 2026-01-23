#include "ResultScene.h"
#include "NullScene.h"
#include "Title.h"
#include "../GameObjects/Attack/AttackManager.h"
#include "../GameObjects/Weapon/WeaponManager.h"
#include "../GameObjects/UI/ScoreUI/ScoreManager.h"
#include "../Componets/HitStopController.h"
#include "../Componets/Input/VirtualController.h"
#include "../Componets/InputMyController/ControllerReceiver.h"

using namespace LWP;
using namespace LWP::Resource;
using namespace LWP::Input;
using namespace LWP::Object;
using namespace LWP::Math;
using namespace LWP::Primitive;
using namespace LWP::Utility;
using namespace LWP::Info;

ResultScene::ResultScene() {
}

ResultScene::~ResultScene() {
	// ゲームコントローラ
	VirtualController::Destroy();
	//マイコン入力の停止
	ControllerReceiver::GetInstance()->ClosePort();
}

void ResultScene::Initialize() {
	//マイコン入力開始
	ControllerReceiver::GetInstance()->ReOpenPort();
	// ゲームコントローラ
	VirtualController::Create();

	// スプライトの設定
	anyKeySprite_.LoadTexture("UI/start_UI.png");
	anyKeySprite_.FitToTexture();
	anyKeySprite_.anchorPoint = { 0.5f, 0.5f };
	anyKeySprite_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,  (LWP::Info::GetWindowHeightF() / 2.0f) + 350.0f };

	backGround_ = std::make_unique<BackGround>();

	//スコア表示テスト
	score_ = std::make_unique<ScoreUI>();
	score_->Initialize(9);

	sceneChangeAnimation_ = std::make_unique<DefaultSceneChangeAnimation>();
	sceneChangeAnimation_->Initialize();
	sceneChangeAnimation_->SetAnimationLength(animationLength_);
	sceneChangeAnimation_->Start(0);
	isChangeScene_ = false;
}

void ResultScene::Update() {
	if (VirtualController::GetInstance()->TriggerAnyKey() || isChangeScene_) {
		ChangeTitleScene();
	}

	//スコア表示(テスト)
	score_->SetScore(ScoreCounter::GetInstance()->GetScore());
	score_->Update();

	sceneChangeAnimation_->Update();

#ifdef _DEBUG
	ImGui::Begin("GameObjects");
	ImGui::BeginTabBar("GameObject");


	ImGui::EndTabBar();
	ImGui::End();
#endif // DEBUG

	// コントローラー
	VirtualController::GetInstance()->Update();
}

void ResultScene::ChangeTitleScene() {
	if (!isChangeScene_) sceneChangeAnimation_->Start(1);
	if (!sceneChangeAnimation_->GetIsPlay()) nextSceneFunction = []() { return new Title(); };
	isChangeScene_ = true;
}