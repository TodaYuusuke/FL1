#include "Title.h"
#include "../Componets/Input/VirtualController.h"
#include "../Componets/InputMyController/ControllerReceiver.h"
#include "../Audio/AudioPlayer.h"
#include "TutorialScene.h"

Title::Title() {

}

Title::~Title() {
	// ゲームコントローラ
	VirtualController::Destroy();
	//マイコン入力の停止
	ControllerReceiver::GetInstance()->ClosePort();
	// 音プレイヤー
	AudioPlayer::GetInstance()->Destroy();
}

// 初期化
void Title::Initialize() {	

#ifndef _DEBUG

	// フルスクリーンに変更
	LWP::Window::ChangeFullScreenMode();

#endif // !_DEBUG

	//マイコン入力開始
	ControllerReceiver::GetInstance()->ReOpenPort();
	// ゲームコントローラ
	VirtualController::Create();

	// 音プレイヤー生成
	AudioPlayer::Create();

	// スプライトの設定
	titleLogoSprite_.LoadTexture("UI/title_logo.png");
	titleLogoSprite_.FitToTexture();
	titleLogoSprite_.anchorPoint = { 0.5f, 0.5f };
	titleLogoSprite_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,  (LWP::Info::GetWindowHeightF() / 2.0f)};

	// スプライトの設定
	anyKeySprite_.LoadTexture("UI/start_UI.png");
	anyKeySprite_.FitToTexture();
	anyKeySprite_.anchorPoint = { 0.5f, 0.5f };
	anyKeySprite_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,  (LWP::Info::GetWindowHeightF() / 2.0f) + 350.0f };

	backGround_ = std::make_unique<BackGround>();

	sceneChangeAnimation_ = std::make_unique<DefaultSceneChangeAnimation>();
	sceneChangeAnimation_->Initialize("BGM_title.mp3");
	sceneChangeAnimation_->SetAnimationLength(animationLength_);
	sceneChangeAnimation_->Start(0);

	isChangeScene_ = false;
}

// 更新
void Title::Update() {
	// 効果音プレイヤー生成
	AudioPlayer::GetInstance()->Update();

	if (VirtualController::GetInstance()->TriggerAnyKey() || isChangeScene_) {
		ChangeTutorialScene();
	}

	// ボタンを点滅させる
	if (isSwitchFade_) {
		if (keySpriteAlpha_ <= 51.0f) {
			isSwitchFade_ = false;
		}
		// 補間で透明度調整
		keySpriteAlpha_ = LWP::Utility::Interp::LerpF(keySpriteAlpha_, 50.0f, 0.1f);
		
	}
	else {
		if (keySpriteAlpha_ >= 254.0f) {
			isSwitchFade_ = true;
		}

		// 補間で透明度調整
		keySpriteAlpha_ = LWP::Utility::Interp::LerpF(keySpriteAlpha_, 255.0f, 0.1f);
	}
	anyKeySprite_.material.color.A = keySpriteAlpha_;

	// コントローラー
	VirtualController::GetInstance()->Update();

	sceneChangeAnimation_->SetAnimationLength(animationLength_);
	sceneChangeAnimation_->Update();

#ifdef _DEBUG
	static bool is;
	// シーン遷移アニメーション
	if (LWP::Input::Keyboard::GetTrigger(DIK_7)) {
		sceneChangeAnimation_->Start(int(is));
		is = !is;
	}

	ImGui::Begin("sceneChange");
	ImGui::DragInt("length",&animationLength_,1);
	ImGui::End();

#endif
}


void Title::ChangeTutorialScene() {
	if (!isChangeScene_) {
		sceneChangeAnimation_->Start(1);
		// 決定音を鳴らす
		AudioPlayer::GetInstance()->PlayAudio("Execute_SE.mp3", 1.0f, LWP::AudioConfig::Other);
	}
	if(!sceneChangeAnimation_->GetIsPlay()) nextSceneFunction = []() { return new TutorialScene(); };
	isChangeScene_ = true;
}