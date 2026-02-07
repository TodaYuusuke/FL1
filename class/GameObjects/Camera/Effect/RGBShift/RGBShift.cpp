#include "RGBShift.h"
#include "../../FollowCamera/FollowCamera.h"
#include "../../CameraConfig.h"

RGBShift::RGBShift(FollowCamera* camera, const LWP::Math::Vector3& shiftRGB)
	:CameraEffect(camera)
{
	// カメラのブラー有効
	pCamera_->camera_->pp.radialBlur.use = true;

	// ブラー強さ設定
	kRGBShiftStrength_ = shiftRGB;
}

void RGBShift::Init()
{
	// 初期値設定
	nowStrength_ = LWP::Math::Vector3();
	nowType = LWP::Utility::Easing::Type::Liner;
	state_ = RGBShiftState::END;
}

void RGBShift::Update()
{
	// カメラのブラー強さ設定
	pCamera_->camera_->pp.rgbShift.shiftR.x = nowStrength_.x;
	pCamera_->camera_->pp.rgbShift.shiftG.x = nowStrength_.y;
	pCamera_->camera_->pp.rgbShift.shiftB.x = nowStrength_.z;

	// タイマー非アクティブ時早期リターン
	if (!timer_.GetIsActive()) {
		// 状態によって分岐
		switch (state_)
		{
		case RGBShiftState::START: // 開始なら最大強さで固定
			nowStrength_ = kRGBShiftStrength_;
			break;
		case RGBShiftState::END: // 終了なら0で固定
			nowStrength_ = LWP::Math::Vector3();
			break;
		}
		return;
	}

	// 状態によって分岐
	switch (state_)
	{
	case RGBShiftState::START:
		// ずらしの強度設定
		nowStrength_ = LWP::Utility::Interp::Lerp(LWP::Math::Vector3(), kRGBShiftStrength_, LWP::Utility::Easing::CallFunction(nowType, timer_.GetProgress()));
		break;
	case RGBShiftState::END:
		// ずらしの強度設定
		nowStrength_ = LWP::Utility::Interp::Lerp(kRGBShiftStrength_, LWP::Math::Vector3(), LWP::Utility::Easing::CallFunction(nowType, timer_.GetProgress()));
		break;
	}

	// タイマー更新
	timer_.Update();
}

void RGBShift::Start(const float startTime, const LWP::Utility::Easing::Type& startType)
{
	// タイマー開始
	timer_.Start(startTime);
	// タイプ取得
	nowType = startType;

	// 開始状態
	state_ = RGBShiftState::START;
}

void RGBShift::Finish(const float endTime, const LWP::Utility::Easing::Type& endType)
{
	// タイマー開始
	timer_.Start(endTime);
	// タイプ取得
	nowType = endType;

	// 終了状態
	state_ = RGBShiftState::END;

	// 現在のずらし強度の取得
	kRGBShiftStrength_ = nowStrength_;
}