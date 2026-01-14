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



}

Anim& AnimationManager::PlayQue(const std::string& animName, const float transitionTime, const bool isLoop)
{
	// 空のアニメーションデータを作成
	LWP::Animation::AnimData animData{};
	// アニメーションデータの初期設定
	animData.AnimName = animName;
	animData.TransitionTime = transitionTime;
	animData.isLoop = isLoop;

	// アニメーション作成
	Anim* newAnim = new Anim(animation_, animData);

	// キュー配列内にアニメーションデータがない場合
	if (animQue_.empty()) {
		// 先頭アニメーションを開始
		newAnim->Start();
	}

	// キュー配列に追加
	animQue_.push_back(newAnim);

	// 先頭のアニメーションを返す
	return *animQue_.back();
}

Anim& AnimationManager::PlayDirect(const std::string& animName, const float transitionTime, const bool isLoop)
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

	// アニメーション作成
	Anim* newAnim = new Anim(animation_, animData);

	// キュー配列に追加
	animQue_.push_front(newAnim);

	// 先頭のアニメーションを返す
	return *animQue_.front();
}

