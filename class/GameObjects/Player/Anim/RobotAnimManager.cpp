#include "RobotAnimManager.h"

RobotAnimManager::RobotAnimManager(const std::string& filePath, LWP::Resource::SkinningModel* model, const LWP::Math::Vector3* moveVec) : AnimationManager(filePath, model)
{
	// 移動ベクトルのアドレス受け取り
	moveVelocity = moveVec;
}

RobotAnimManager::~RobotAnimManager()
{
	// 部位それぞれのキュー内の全データ解放
	for (Anim* anim : leftArmAnimQue_) {
		delete anim;
	}
	leftArmAnimQue_.clear();
	for (Anim* anim : rightArmAnimQue_) {
		delete anim;
	}
	rightArmAnimQue_.clear();
	for (Anim* anim : leftShoulderAnimQue_) {
		delete anim;
	}
	leftShoulderAnimQue_.clear();
	for (Anim* anim : rightShoulderAnimQue_) {
		delete anim;
	}
	rightShoulderAnimQue_.clear();
}

void RobotAnimManager::Init()
{
	// 基底クラスの初期化処理呼び出し
	AnimationManager::Init();
	
	// 部位それぞれのキュー内の全データ解放
	for (Anim* anim : leftArmAnimQue_) {
		delete anim;
	}
	leftArmAnimQue_.clear();
	for (Anim* anim : rightArmAnimQue_) {
		delete anim;
	}
	rightArmAnimQue_.clear();
	for (Anim* anim : leftShoulderAnimQue_) {
		delete anim;
	}
	leftShoulderAnimQue_.clear();
	for (Anim* anim : rightShoulderAnimQue_) {
		delete anim;
	}
	rightShoulderAnimQue_.clear();

	// 移動ブレンド用アニメーションの開始と停止
	animation_.Play("Other_Move", 0.0f, 0.0f, LWP::Resource::Animation::TrackType::Blend);
	animation_.Loop(true, LWP::Resource::Animation::TrackType::Blend);
	animation_.Stop(LWP::Resource::Animation::TrackType::Blend);
}

void RobotAnimManager::Update()
{
	// 停止する場合早期リターン
	if (isStop_) { return; }

	// アニメーション更新
	animation_.Update();

	// 腕以外のキューを更新
	AnimQueUpdate(animQue_);
	// 部位それぞれのキューを更新
	AnimQueUpdate(leftArmAnimQue_);
	AnimQueUpdate(rightArmAnimQue_);
	AnimQueUpdate(leftShoulderAnimQue_);
	AnimQueUpdate(rightShoulderAnimQue_);

	// 移動ブレンドの更新
	MoveBlendUpdate();
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

	// 空のアニメーション作成
	Anim* newAnim = nullptr;

	// 再生する場所によって処理を変更する
	switch (trackType)
	{
	case RobotAnimManager::Other:
		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// キューが空なら再生する
		if (animQue_.empty()) { newAnim->Start(); }

		// キュー配列に追加
		animQue_.push_back(newAnim);

		// 末尾のアニメーションを返す
		return *animQue_.back();
		break;
	case RobotAnimManager::LeftArm:
		// 先頭に識別子追加
		animData.AnimName = "HandL_" + animName;
		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// キューが空なら再生する
		if (leftArmAnimQue_.empty()) { newAnim->Start(); }

		// キュー配列に追加
		leftArmAnimQue_.push_back(newAnim);

		// 末尾のアニメーションを返す
		return *leftArmAnimQue_.back();
		break;
	case RobotAnimManager::RightArm:
		// 先頭に識別子追加
		animData.AnimName = "HandR_" + animName;
		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// キューが空なら再生する
		if (rightArmAnimQue_.empty()) { newAnim->Start(); }

		// キュー配列に追加
		rightArmAnimQue_.push_back(newAnim);

		// 末尾のアニメーションを返す
		return *rightArmAnimQue_.back();
		break;
	case RobotAnimManager::LeftShoulder:
		// 先頭に識別子追加
		animData.AnimName = "ShoulderL_" + animName;
		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// キューが空なら再生する
		if (leftShoulderAnimQue_.empty()) { newAnim->Start(); }

		// キュー配列に追加
		leftShoulderAnimQue_.push_back(newAnim);

		// 末尾のアニメーションを返す
		return *leftShoulderAnimQue_.back();
		break;
	case RobotAnimManager::RightShoulder:
		// 先頭に識別子追加
		animData.AnimName = "ShoulderR_" + animName;
		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// キューが空なら再生する
		if (rightShoulderAnimQue_.empty()) { newAnim->Start(); }

		// キュー配列に追加
		rightShoulderAnimQue_.push_back(newAnim);

		// 末尾のアニメーションを返す
		return *rightShoulderAnimQue_.back();
		break;

	}

	// ここまで来てしまったらエラー
	assert(false);

	// ここまで来ることは本来ないはずだが一応値を返す
	return *animQue_.back();
}

Anim& RobotAnimManager::PlayDirect(const std::string& animName, const int trackType, const float transitionTime, const bool isLoop)
{
	// 空のアニメーションデータを作成
	LWP::Animation::AnimData animData{};
	// アニメーションデータの初期設定
	animData.AnimName = animName;
	animData.TransitionTime = transitionTime;
	animData.isLoop = isLoop;
	animData.trackType = trackType;

	// アニメーション作成
	Anim* newAnim = nullptr;

	// 再生する場所によって処理を変更する
	switch (trackType)
	{
	case RobotAnimManager::Other:
		// キュー配列内にアニメーションデータがある場合
		if (!animQue_.empty()) {
			// 先頭アニメーションを破棄
			delete animQue_.front();
			animQue_.pop_front();
		}

		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// 強制的に再生する
		newAnim->Start();

		// キュー配列に追加
		animQue_.push_front(newAnim);

		// 先頭のアニメーションを返す
		return *animQue_.front();
		break;

	case RobotAnimManager::LeftArm:
		// キュー配列内にアニメーションデータがある場合
		if (!leftArmAnimQue_.empty()) {
			// 先頭アニメーションを破棄
			delete leftArmAnimQue_.front();
			leftArmAnimQue_.pop_front();
		}

		// 先頭に識別子追加
		animData.AnimName = "HandL_" + animName;

		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// 強制的に再生する
		newAnim->Start();

		// キュー配列に追加
		leftArmAnimQue_.push_front(newAnim);

		// 先頭のアニメーションを返す
		return *leftArmAnimQue_.front();
		break;

	case RobotAnimManager::RightArm:
		// キュー配列内にアニメーションデータがある場合
		if (!rightArmAnimQue_.empty()) {
			// 先頭アニメーションを破棄
			delete rightArmAnimQue_.front();
			rightArmAnimQue_.pop_front();
		}

		// 先頭に識別子追加
		animData.AnimName = "HandR_" + animName;

		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// 強制的に再生する
		newAnim->Start();

		// キュー配列に追加
		rightArmAnimQue_.push_front(newAnim);

		// 先頭のアニメーションを返す
		return *rightArmAnimQue_.front();
		break;

	case RobotAnimManager::LeftShoulder:
		// キュー配列内にアニメーションデータがある場合
		if (!leftShoulderAnimQue_.empty()) {
			// 先頭アニメーションを破棄
			delete leftShoulderAnimQue_.front();
			leftShoulderAnimQue_.pop_front();
		}

		// 先頭に識別子追加
		animData.AnimName = "ShoulderL_" + animName;

		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// 強制的に再生する
		newAnim->Start();

		// キュー配列に追加
		leftShoulderAnimQue_.push_front(newAnim);

		// 先頭のアニメーションを返す
		return *leftShoulderAnimQue_.front();
		break;

	case RobotAnimManager::RightShoulder:
		// キュー配列内にアニメーションデータがある場合
		if (!rightShoulderAnimQue_.empty()) {
			// 先頭アニメーションを破棄
			delete rightShoulderAnimQue_.front();
			rightShoulderAnimQue_.pop_front();
		}

		// 先頭に識別子追加
		animData.AnimName = "ShoulderR_" + animName;

		// アニメーション作成
		newAnim = new Anim(&animation_, animData);

		// 強制的に再生する
		newAnim->Start();

		// キュー配列に追加
		rightShoulderAnimQue_.push_front(newAnim);

		// 先頭のアニメーションを返す
		return *rightShoulderAnimQue_.front();
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
	animation_.SetTime(CalcMoveT(moveV), LWP::Resource::Animation::TrackType::Blend);
	// 移動ベクトルの長さによってブレンドTを変更する
	animation_.blendT = std::clamp<float>(moveV.Length(), 0.0f, 1.0f);
}

float RobotAnimManager::CalcMoveT(const LWP::Math::Vector2& v)
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
