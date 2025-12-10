#include "Title.h"
#include "../Componets/Input/VirtualController.h"
#include "../Componets/InputMyController/ControllerReceiver.h"
#include "GameScene.h"

Title::Title() {
}

Title::~Title() {
	// ゲームコントローラ
	VirtualController::Destroy();
	//マイコン入力の停止
	ControllerReceiver::GetInstance()->ClosePort();
}

// 初期化
void Title::Initialize() {	
	// ゲームコントローラ
	VirtualController::Create();
}

// 更新
void Title::Update() {
	// コントローラー
	VirtualController::GetInstance()->Update();

	if (VirtualController::GetInstance()->TriggerAnyKey()) {
		nextSceneFunction = []() { return new GameScene(); };
	}
}