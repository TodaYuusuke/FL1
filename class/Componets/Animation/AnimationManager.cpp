#include "AnimationManager.h"

AnimationManager::AnimationManager(LWP::Resource::Animation* anim)
{
	// ポインタの受け取り
	animation_ = anim;
	// アニメーション初期化
	animation_->Init();
}

AnimationManager::~AnimationManager()
{
	// キュー内の全データ解放
	for (Anim* anim : animQue_) {
		delete anim;
	}
	animQue_.clear();
}

void AnimationManager::Init() {
	// アニメーション初期化
	animation_->Init();

	// キュー内の全データ解放
	for (Anim* anim : animQue_) {
		delete anim;
	}
	animQue_.clear();
}

void AnimationManager::Update() {
	
	// 停止する場合早期リターン
	if (isStop_) { return;  }
	
	// アニメーション更新
	animation_->Update();

	//キュー内のデータが無ければ早期リターン
	if (animQue_.empty()) { return; }
	
	// キュー先頭のアニメーションを取得
	Anim* currentAnim = animQue_.front();
	// 先頭アニメーションの更新
	currentAnim->Update();

	//終了した場合
	if (currentAnim->GetIsEnd()) {
		// 先頭アニメーションを破棄
		delete currentAnim;
		animQue_.pop_front();

		// 次のアニメーションが登録されている場合それを再生する
		if (!animQue_.empty()) {
			animQue_.front()->Start();
		}
	}
}

void AnimationManager::DebugGUI(const std::string& id) {
	// デバッグメニューの表示
	std::string displayName = "Animation Debug Menu - " + id;
	ImGui::SeparatorText(displayName.c_str());

	// 全アニメーションの停止
	ImGui::NewLine();
	ImGui::Checkbox("Stop Play", &isStop_);
	ImGui::NewLine();

	// キュー内のアニメーションを表示
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 200), ImGuiWindowFlags_NoTitleBar);
	
	// アニメーションキューが空でない場合
	if (!animQue_.empty()) {
		// 一時変数
		int editID = 0;
		Anim* change = nullptr;

		for (Anim* a : animQue_) {
			a->ImGuiRadioButton(selectedID_, editID, change);
		}

		// 表示対象が変更されていた場合
		if (change != nullptr) {
			debugingAnim_ = change;
		}
	}
	else {
		// キューが空であることを示す
		ImGui::Text("Que is Empty.");
		// 選択アニメーションはnullに
		debugingAnim_ = nullptr;
		// 選択IDリセット
		selectedID_ = 0;
	}

	ImGui::EndChild();

	// アニメーションが選択されている場合
	if (debugingAnim_ != nullptr) {
		// そのGUIを表示
		debugingAnim_->DebugGUI();
	}else{
		ImGui::NewLine();
		ImGui::Text("No Selected!");
		ImGui::NewLine();
	}
}

Anim& AnimationManager::PlayQue(const std::string& animName, const float transitionTime, const bool isLoop, const int trackType)
{
	// 空のアニメーションデータを作成
	LWP::Animation::AnimData animData{};
	// アニメーションデータの初期設定
	animData.AnimName = animName;
	animData.TransitionTime = transitionTime;
	animData.isLoop = isLoop;
	animData.trackType = trackType;

	// アニメーション作成
	Anim* newAnim = new Anim(animation_, animData);

	// キューが空なら再生する
	if (animQue_.empty()) { newAnim->Start(); }

	// キュー配列に追加
	animQue_.push_back(newAnim);

	// 先頭のアニメーションを返す
	return *animQue_.back();
}

Anim& AnimationManager::PlayDirect(const std::string& animName, const float transitionTime, const bool isLoop, const int trackType)
{
	// キュー配列内にアニメーションデータがある場合
	if (!animQue_.empty()) {
		// 先頭アニメーションを破棄
		delete animQue_.front();
		animQue_.pop_front();
	}

	// 空のアニメーションデータを作成
	LWP::Animation::AnimData animData{};
	// アニメーションデータの初期設定
	animData.AnimName = animName;
	animData.TransitionTime = transitionTime;
	animData.isLoop = isLoop;
	animData.trackType = trackType;

	// アニメーション作成
	Anim* newAnim = new Anim(animation_, animData);

	// 強制的に再生する
	newAnim->Start();

	// キュー配列に追加
	animQue_.push_front(newAnim);

	// 先頭のアニメーションを返す
	return *animQue_.front();
}
