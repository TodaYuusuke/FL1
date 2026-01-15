#include "TestScene.h"
#include "Title.h"

using namespace LWP;
using namespace LWP::Input;

TestScene::TestScene()
{
	// インスタンス生成
	EffectManager::Create();
	EffectEditor::Create();
}

TestScene::~TestScene()
{
	// インスタンス生成
	EffectManager::Destroy();
	EffectEditor::Destroy();

	// アニメーションマネージャー削除
	delete animationManager_;
}

// 初期化
void TestScene::Initialize() {
	// エフェクト関連初期化
	EffectManager::GetInstance()->Init();
	EffectEditor::GetInstance()->SetEffectManager(EffectManager::GetInstance());
	EffectEditor::GetInstance()->Init();

	robotModel_.LoadShortPath("Player/Player.gltf");
	animation_.LoadFullPath("resources/model/Player/Player.gltf", &robotModel_);

	animationManager_ = new AnimationManager(&animation_);
	animationManager_->Init();

	testSE_.LoadShortPath("Shot.mp3");
	testSE2_.LoadShortPath("PowerShot.mp3");

	testModel_.LoadCube();				// 立方体と球はデフォルトで用意してある
}

// 更新
void TestScene::Update() {
	// エフェクト関連更新
	EffectManager::GetInstance()->Update();
	EffectEditor::GetInstance()->Update();
	
	animationManager_->Update();

	ImGui::Begin("Test");

	animationManager_->DebugGUI("Test");

	if (ImGui::Button("Play HandL_Shot_Que")) {
		animationManager_->PlayQue("HandL_Shot", 0.1f, false)
			.AddEvent("PlaySE", 1, [&]() { testSE_.Play(); });
	}
	
	if (ImGui::Button("Play HandL_PowerShot_Que")) {
		animationManager_->PlayQue("HandL_PowerShot", 0.1f, false)
			.AddEvent("PlaySE", 1, [&]() { testSE2_.Play(); });
	}

	if (ImGui::Button("Play HandL_Shot_Direct")) {
		animationManager_->PlayDirect("HandL_Shot", 0.1f, false)
			.AddEvent("PlaySE", 1, [&]() { testSE_.Play(); })
			.Start();
	}

	if (ImGui::Button("Play HandL_PowerShot_Direct")) {
		animationManager_->PlayDirect("HandL_PowerShot", 0.1f, false)
			.AddEvent("PlaySE", 1, [&]() { testSE2_.Play(); })
			.Start();
	}


	ImGui::End();
}

