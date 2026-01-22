#include "RobotAnimManager.h"

RobotAnimManager::RobotAnimManager(LWP::Resource::Animation* anim, const LWP::Math::Vector3* moveVec) : AnimationManager(anim)
{
	// 移動ベクトルのアドレス受け取り
	moveVelocity = moveVec;
}

RobotAnimManager::~RobotAnimManager()
{
	// 右腕用、左腕用キュー内の全データ解放
	for (Anim* anim : leftAnimQue_) {
		delete anim;
	}
	leftAnimQue_.clear();
	for (Anim* anim : rightAnimQue_) {
		delete anim;
	}
	rightAnimQue_.clear();
}

void RobotAnimManager::Init()
{
	// 基底クラスの初期化処理呼び出し
	AnimationManager::Init();
	
	// 右腕用、左腕用キュー内の全データ解放
	for (Anim* anim : leftAnimQue_) {
		delete anim;
	}
	leftAnimQue_.clear();
	for (Anim* anim : rightAnimQue_) {
		delete anim;
	}
	rightAnimQue_.clear();
}

void RobotAnimManager::Update()
{
	// 停止する場合早期リターン
	if (isStop_) { return; }

	// アニメーション更新
	animation_->Update();

	// 腕以外のキューを更新
	AnimQueUpdate(animQue_);
	// 左腕、右腕それぞれのキューを更新
	AnimQueUpdate(leftAnimQue_);
	AnimQueUpdate(rightAnimQue_);
}

Anim& RobotAnimManager::PlayQue(const std::string& animName, const int trackType, const float transitionTime, const bool isLoop)
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

	// 再生する場所によって処理を変更する
	switch (trackType)
	{
	case RobotAnimManager::Other:
		// キューが空なら再生する
		if (animQue_.empty()) { newAnim->Start(); }

		// キュー配列に追加
		animQue_.push_back(newAnim);

		// 末尾のアニメーションを返す
		return *animQue_.back();
		break;
	case RobotAnimManager::RightArm:
		// キューが空なら再生する
		if (rightAnimQue_.empty()) { newAnim->Start(); }

		// キュー配列に追加
		rightAnimQue_.push_back(newAnim);

		// 末尾のアニメーションを返す
		return *rightAnimQue_.back();
		break;
	case RobotAnimManager::LeftArm:
		// キューが空なら再生する
		if (leftAnimQue_.empty()) { newAnim->Start(); }

		// キュー配列に追加
		leftAnimQue_.push_back(newAnim);

		// 末尾のアニメーションを返す
		return *leftAnimQue_.back();
		break;
	}

	// ここまで来てしまったらエラー
	assert(false);

	// ここまで来ることは本来ないはずだが一応値を返す
	return *animQue_.back();
}

Anim& RobotAnimManager::PlayDirect(const std::string& animName, const int trackType, const float transitionTime, const bool isLoop)
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

	// 再生する場所によって処理を変更する
	switch (trackType)
	{
	case RobotAnimManager::Other:
		// 強制的に再生する
		newAnim->Start();

		// キュー配列に追加
		animQue_.push_front(newAnim);

		// 先頭のアニメーションを返す
		return *animQue_.front();
		break;
	case RobotAnimManager::RightArm:
		// 強制的に再生する
		newAnim->Start();

		// キュー配列に追加
		rightAnimQue_.push_front(newAnim);

		// 先頭のアニメーションを返す
		return *rightAnimQue_.front();
		break;
	case RobotAnimManager::LeftArm:
		// 強制的に再生する
		newAnim->Start();

		// キュー配列に追加
		leftAnimQue_.push_front(newAnim);

		// 先頭のアニメーションを返す
		return *leftAnimQue_.front();
		break;
	}

	// ここまで来てしまったらエラー
	assert(false);

	// ここまで来ることは本来ないが一応値を返す
	return *animQue_.front();
}

void RobotAnimManager::AnimQueUpdate(std::list<Anim*>& animQue)
{
	//キュー内のデータが無ければ早期リターン
	if (animQue.empty()) { return; }

	// キュー先頭のアニメーションを取得
	Anim* currentAnim = animQue.front();
	// 先頭アニメーションの更新
	currentAnim->Update();

	//終了した場合
	if (currentAnim->GetIsEnd()) {
		// 先頭アニメーションを破棄
		delete currentAnim;
		animQue.pop_front();

		// 次のアニメーションが登録されている場合それを再生する
		if (!animQue.empty()) {
			animQue.front()->Start();
		}
	}
}

void RobotAnimManager::MoveBlendUpdate()
{
	// 移動ベクトルを二次元ベクトルで取得
	LWP::Math::Vector2 moveV = LWP::Math::Vector2(moveVelocity->x, moveVelocity->z);

	// ブレンド用tを求め、アニメーションのBlendTにセット
	animation_->blendT = CalcVecBlendT(moveV);
}

float RobotAnimManager::CalcVecBlendT(const LWP::Math::Vector2& v)
{
	// 各軸の絶対値を求める
	float ax = std::abs(v.x);
	float ay = std::abs(v.y);

	// 合成
	float sum = ax + ay;
	if (sum < 1e-6f) {
		return 0.0f;
	}

	// t を求める
	float t = ay / sum;

	// 各軸の値を元に補正
	if (v.x < 0.0f) {
		t = 1.0f - t;
	}
	if (v.y < 0.0f) {
		t = 1.0f + t;
	}

	// 求めた値を返す
	return t * 0.25f;
}
