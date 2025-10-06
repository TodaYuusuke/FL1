#include "TestScene.h"
#include "Title.h"

using namespace LWP;
using namespace LWP::Input;

// 初期化
void TestScene::Initialize() {
	testModel_.LoadCube();				// 立方体と球はデフォルトで用意してある
	sprite_.LoadTexture("uvChecker.png");	// resources/texture直下のパスを指定する
}

// 更新
void TestScene::Update() {
	// 次のシーンへ以降
	if (Input::Keyboard::GetTrigger(DIK_P)) {
		nextSceneFunction = []() { return new Title(); };
	}

	// 立方体のデバッグGUI
	ImGui::Begin("Test");
	if(ImGui::TreeNode("Cube Model")) {
		testModel_.DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sprite")) {
		sprite_.DebugGUI();
		ImGui::TreePop();
	}
	ImGui::End();
}