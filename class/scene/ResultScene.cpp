#include "ResultScene.h"
#include "NullScene.h"
#include "Title.h"
#include "../GameObjects/Attack/AttackManager.h"
#include "../GameObjects/Weapon/WeaponManager.h"
#include "../Componets/HitStopController.h"
#include "../GameObjects/UI/ScoreUI/ScoreManager.h"

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

}

void ResultScene::Initialize() {
	anyKeySprite_.LoadTexture("UI/start_UI.png");
	anyKeySprite_.FitToTexture();
	anyKeySprite_.anchorPoint = { 0.5f, 0.5f };
	anyKeySprite_.worldTF.translation = { LWP::Info::GetWindowWidthF() / 2.0f,  (LWP::Info::GetWindowHeightF() / 2.0f) + 350.0f };

	//スコア表示テスト
	score_ = std::make_unique<ScoreUI>();
	score_->Initialize(9);
}

void ResultScene::Update() {
	
	//スコア表示(テスト)
	score_->SetScore(ScoreCounter::GetInstance()->GetScore());
	score_->Update();


#ifdef _DEBUG
	ImGui::Begin("GameObjects");
	ImGui::BeginTabBar("GameObject");


	ImGui::EndTabBar();
	ImGui::End();
#endif // DEBUG
}
