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

	// 音プレイヤー
	AudioPlayer::Destroy();
}

void ResultScene::Initialize() {
	//マイコン入力開始
	ControllerReceiver::GetInstance()->ReOpenPort();
	// ゲームコントローラ
	VirtualController::Create();

	// 音プレイヤー
	AudioPlayer::Create();

	// スプライトの設定
	anyKeySprite_.LoadTexture("UI/start_UI.png");
	anyKeySprite_.FitToTexture();
	anyKeySprite_.anchorPoint = { 0.5f, 0.5f };
	anyKeySprite_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,  (LWP::Info::GetWindowHeightF() / 2.0f) + 350.0f };
	anyKeySprite_.isActive = false;

	backGround_ = std::make_unique<BackGround>();
	backGround_->SetColor({0.0f,0.0f,0.0f,1.0f});

	//スコア表示テスト
	score_ = std::make_unique<ScoreUI>();
	score_->Initialize(9);
	score_->SetIsScoreDisplay(false);
	ScoreCounter::GetInstance()->ClearDrawScore();
	
	//仮値
	//ｊScoreCounter::GetInstance()->AddScore(12345);

	knockOut_ = std::make_unique<ScoreUI>();
	knockOut_->Initialize(3);
	knockOut_->SetIsScoreDisplay(false);

	//撃破数を取ってくる
	killCount_ = ScoreCounter::GetInstance()->GetKillCount();
	drawKillCount_ = 0;

	sceneChangeAnimation_ = std::make_unique<DefaultSceneChangeAnimation>();
	sceneChangeAnimation_->Initialize("BGM_result.mp3");
	sceneChangeAnimation_->SetAnimationLength(animationLength_);
	//sceneChangeAnimation_->Start(0);
	isChangeScene_ = false;

	state_ = std::bind(&ResultScene::Phase1KnockOut,this);

	killCountSprite_.LoadTexture("Result/killcount_UI.png");
	killCountSprite_.isActive = false;

	scoreSprite_.LoadTexture("Result/score_UI.png");
	scoreSprite_.isActive = false;

	scoreAnimationFrame_ = 0;

	// 管理クラスの調整項目
	json_.Init(kJsonDirectoryPath + "Result.json")
		.BeginGroup("KillCount")
		.BeginGroup("Text")
		.AddValue<LWP::Math::Vector3>("Transform", &killCountSprite_.worldTF.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &killCountSprite_.worldTF.scale)
		.EndGroup()
		.BeginGroup("Num")
		.AddValue<LWP::Math::Vector3>("Transform", &killCountTransform_.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &killCountTransform_.scale)
		.EndGroup()
		.EndGroup()
		.BeginGroup("Score")
		.BeginGroup("Text")
		.AddValue<LWP::Math::Vector3>("Transform", &scoreSprite_.worldTF.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &scoreSprite_.worldTF.scale)
		.EndGroup()
		.BeginGroup("Num")
		.AddValue<LWP::Math::Vector3>("Transform", &scoreTransform_.translation)
		.AddValue<LWP::Math::Vector3>("Scale", &scoreTransform_.scale)
		.EndGroup()
		.AddValue<float>("AnimationLength", &scoreAnimationLength_)
		.EndGroup()
		.CheckJsonFile();
}

void ResultScene::Update() {
	// 効果音プレイヤー生成
	AudioPlayer::GetInstance()->Update();

	if (VirtualController::GetInstance()->TriggerAnyKey() || isChangeScene_) {
		//ChangeTitleScene();
	}

	knockOut_->SetCenter({ killCountTransform_.translation.x,killCountTransform_.translation.y });
	knockOut_->Update();

	//スコア表示(テスト)
	score_->SetCenter(scoreTransform_.translation.xy());
	score_->Update();

	state_();

	sceneChangeAnimation_->Update();

#ifdef _DEBUG
	ImGui::Begin("GameObjects");
	ImGui::BeginTabBar("GameObject");
	if (ImGui::BeginTabItem("Result")) {
		json_.DebugGUI();
		ImGui::EndTabItem();
	}

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

void ResultScene::Phase1KnockOut() {
	knockOut_->SetIsScoreDisplay(true);
	killCountSprite_.isActive = true;

	knockOut_->SetScore(drawKillCount_++);
	if (VirtualController::GetInstance()->TriggerAnyKey()) {
		drawKillCount_ = killCount_;
	}
	if (killCount_ < drawKillCount_) {
		//次のフェーズへ
		state_ = std::bind(&ResultScene::Phase2Score, this);
		return;
	}
}

void ResultScene::Phase2Score() {
	score_->SetIsScoreDisplay(true);
	scoreSprite_.isActive = true;

	float t = scoreAnimationFrame_ / scoreAnimationLength_;
	scoreAnimationFrame_++;

	if (scoreAnimationFrame_ > scoreAnimationLength_) {
		scoreAnimationFrame_ = scoreAnimationLength_;
	}

	//ScoreCounter::GetInstance()->Update();
	ScoreCounter::GetInstance()->SetDrawScore(ScoreCounter::GetInstance()->GetScore() * t);
	if (VirtualController::GetInstance()->TriggerAnyKey()) {
		ScoreCounter::GetInstance()->SetMaxDrawScore();
		scoreAnimationFrame_ = scoreAnimationLength_;
	}
	score_->SetScore(ScoreCounter::GetInstance()->GetDrawScore());
	if (ScoreCounter::GetInstance()->GetDrawScore() == ScoreCounter::GetInstance()->GetScore()) {
		//次のフェーズへ
		state_ = std::bind(&ResultScene::Phase3ToTitle, this);
	}
}

void ResultScene::Phase3ToTitle() {
	anyKeySprite_.isActive = true;
	if (VirtualController::GetInstance()->TriggerAnyKey() || isChangeScene_) {
		ChangeTitleScene();
	}
}