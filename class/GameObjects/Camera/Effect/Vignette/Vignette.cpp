#include "Vignette.h"
#include "../../FollowCamera/FollowCamera.h"
#include "../../CameraConfig.h"

Vignette::Vignette(FollowCamera* camera, const float blurStrength)
	: CameraEffect(camera)
{
	// カメラのブラー有効
	pCamera_->camera_->pp.radialBlur.use = true;

	// ブラー強さ設定
	kVignetteStrength_ = blurStrength;
}

void Vignette::Init()
{
	// 初期値設定
	nowStrength_ = 0.0f;
	nowType = LWP::Utility::Easing::Type::Liner;
	state_ = VignetteState::END;
}

void Vignette::Update()
{
	// タイマー非アクティブ時早期リターン
	if (!timer_.GetIsActive()) {
		// 状態によって分岐
		switch (state_)
		{
		case VignetteState::START: // 開始なら最大強さで固定
			nowStrength_ = kVignetteStrength_;
			break;
		case VignetteState::END: // 終了なら0で固定
			nowStrength_ = 0.0f;
			break;
		}

		// カメラのブラー強さ設定
		pCamera_->camera_->pp.vignetting.intensity = nowStrength_;

		return;
	}

	// 状態によって分岐
	switch (state_)
	{
	case VignetteState::START:
		// ブラーの強さ設定
		nowStrength_ = LWP::Utility::Interp::LerpF(0.0f, kVignetteStrength_, LWP::Utility::Easing::CallFunction(nowType, timer_.GetProgress()));
		break;
	case VignetteState::END:
		// ブラーの強さ設定
		nowStrength_ = LWP::Utility::Interp::LerpF(kVignetteStrength_, 0.0f, LWP::Utility::Easing::CallFunction(nowType, timer_.GetProgress()));
		break;
	}

	// タイマー更新
	timer_.Update();

	// カメラのブラー強さ設定
	pCamera_->camera_->pp.vignetting.intensity = nowStrength_;
}

void Vignette::Start(const float startTime, const LWP::Utility::Easing::Type& startType)
{
	// タイマー開始
	timer_.Start(startTime);
	// タイプ取得
	nowType = startType;

	// 開始状態
	state_ = VignetteState::START;
}

void Vignette::Finish(const float endTime, const LWP::Utility::Easing::Type& endType)
{
	// タイマー開始
	timer_.Start(endTime);
	// タイプ取得
	nowType = endType;

	// 終了状態
	state_ = VignetteState::END;

	// ブラーの現在強さ取得
	kVignetteStrength_ = nowStrength_;
}