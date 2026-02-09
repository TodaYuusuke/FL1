#include "CameraEffectHandler.h"
#include "../../../Componets/Math.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace FLMath;

CameraEffectHandler::CameraEffectHandler() {
	
}

CameraEffectHandler::~CameraEffectHandler() {
	delete blurEffector_;
	delete vignetteEffector_;
}

void CameraEffectHandler::Init() {
	for (auto& effect : cameraEffects_) {
		effect.second->Init();
	}

	// 各種ポストプロセス生成
	blurEffector_ = new Blur(effectTarget_, 0.0f);
	vignetteEffector_ = new Vignette(effectTarget_, 0.0f);
	rgbShiftEffector_ = new RGBShift(effectTarget_, LWP::Math::Vector3());
}

void CameraEffectHandler::Update() {
	for (auto& effect : cameraEffects_) {
		effect.second->Update();
	}

	// ブラーポストプロセス更新
	if (blurEffector_ != nullptr) {
		blurEffector_->Update();
	}

	// ビネットポストプロセス更新
	if (vignetteEffector_ != nullptr) {
		vignetteEffector_->Update();
	}

	// RGBずらしポストプロセス更新
	if (rgbShiftEffector_ != nullptr) {
		rgbShiftEffector_->Update();
	}
}

void CameraEffectHandler::StartShake(Vector3 range, float endTime) {
	// 生成されていたら削除
	if (cameraEffects_[CameraEffectType::kShake]) { cameraEffects_.erase(CameraEffectType::kShake); }

	// 生成
	cameraEffects_[CameraEffectType::kShake] = std::make_unique<CameraShake>(effectTarget_, range, endTime);
}

void CameraEffectHandler::StartZoom(float zoomValue, float endTime) {
	// 生成されていたら削除
	if (cameraEffects_[CameraEffectType::kZoom]) { cameraEffects_.erase(CameraEffectType::kZoom); }

	// 生成
	cameraEffects_[CameraEffectType::kZoom] = std::make_unique<CameraZoom>(effectTarget_, zoomValue, endTime);
}

void CameraEffectHandler::StartBound(Vector3 boundValue, float endTime) {
	// 生成されていたら削除
	if (cameraEffects_[CameraEffectType::kBound]) { cameraEffects_.erase(CameraEffectType::kBound); }
	
	// 生成
	cameraEffects_[CameraEffectType::kBound] = std::make_unique<CameraBound>(effectTarget_, boundValue, endTime);
}
