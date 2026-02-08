#include "TestScene.h"
#include "Title.h"

using namespace LWP;
using namespace LWP::Input;

TestScene::TestScene()
{
	// インスタンス生成
	EffectManager::Create();
	EffectEditor::Create();
	AudioPlayer::Create();
}

TestScene::~TestScene()
{
	// インスタンス生成
	EffectManager::Destroy();
	EffectEditor::Destroy();
	AudioPlayer::Destroy();

}

// 初期化
void TestScene::Initialize() {
	// エフェクト関連初期化
	EffectManager::GetInstance()->Init();
	EffectEditor::GetInstance()->SetEffectManager(EffectManager::GetInstance());
	EffectEditor::GetInstance()->Init();

	

	testModel_.LoadShortPath("Cockpit/Cockpit.gltf");				// 立方体と球はデフォルトで用意してある
}

// 更新
void TestScene::Update() {
	// エフェクト関連更新
	EffectManager::GetInstance()->Update();
	EffectEditor::GetInstance()->Update();

	testModel_.DebugGUI();
}

