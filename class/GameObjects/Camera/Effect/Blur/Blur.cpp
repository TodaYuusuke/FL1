#include "Blur.h"
#include "../../FollowCamera/FollowCamera.h"
#include "../../CameraConfig.h"

Blur::Blur(FollowCamera* camera, const float blurStrength)
	: CameraEffect(camera)
{
	// カメラのブラー有効
	pCamera_->camera_->pp.radialBlur.use = true;

	// ブラー強さ設定
	kBlurStrength_ = blurStrength;
}

void Blur::Init()
{
	// 初期値設定
	nowStrength_ = 0.0f;
	nowType = LWP::Utility::Easing::Type::Liner;
	state_ = BlurState::END;
}

void Blur::Update()
{
	// カメラのブラー強さ設定
	pCamera_->camera_->pp.radialBlur.blurWidth = nowStrength_;

	// タイマー非アクティブ時早期リターン
	if (!timer_.GetIsActive()) { 
		// 状態によって分岐
		switch (state_)
		{
		case BlurState::START: // 開始なら最大強さで固定
			nowStrength_ = kBlurStrength_;
			break;
		case BlurState::END: // 終了なら0で固定
			nowStrength_ = 0.0f;
			break;
		}
		return; 
	}

	// 状態によって分岐
	switch (state_)
	{
	case BlurState::START:
		// ブラーの強さ設定
		nowStrength_ = LWP::Utility::Interp::LerpF(0.0f, kBlurStrength_, LWP::Utility::Easing::CallFunction(nowType, timer_.GetProgress()));
		break;
	case BlurState::END:
		// ブラーの強さ設定
		nowStrength_ = LWP::Utility::Interp::LerpF(kBlurStrength_, 0.0f, LWP::Utility::Easing::CallFunction(nowType, timer_.GetProgress()));
		break;
	}

	// タイマー更新
	timer_.Update();
}

void Blur::Start(const float startTime, const LWP::Utility::Easing::Type& startType)
{
	// タイマー開始
	timer_.Start(startTime);
	// タイプ取得
	nowType = startType;

	// 開始状態
	state_ = BlurState::START;
}

void Blur::Finish(const float endTime, const LWP::Utility::Easing::Type& endType)
{
	// タイマー開始
	timer_.Start(endTime);
	// タイプ取得
	nowType = endType;

	// 終了状態
	state_ = BlurState::END;

	// ブラーの現在強さ取得
	kBlurStrength_ = nowStrength_;
}
