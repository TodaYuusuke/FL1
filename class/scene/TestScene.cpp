#include "TestScene.h"
#include "Title.h"

using namespace LWP;
using namespace LWP::Input;

// 初期化
void TestScene::Initialize() {
	// インスタンス作成
	effectManager_ = std::make_unique<EffectManager>();
	effectManager_->Init();

	testModel_.LoadCube();				// 立方体と球はデフォルトで用意してある
}

// 更新
void TestScene::Update() {
	
	effectManager_->Update();
	effectManager_->DebugGUI();

	//// 次のシーンへ以降
	//if (Input::Keyboard::GetTrigger(DIK_P)) {
	//	nextSceneFunction = []() { return new Title(); };
	//}

	
}