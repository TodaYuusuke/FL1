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
}

void CameraEffectHandler::Init() {
	for (auto& effect : cameraEffects_) {
		effect.second->Init();
	}

	// ブラーのエフェクト生成
	blurEffector_ = new Blur(effectTarget_, 0.0f);
}

void CameraEffectHandler::Update() {
	for (auto& effect : cameraEffects_) {
		effect.second->Update();
	}

	// ブラーエフェクト更新
	if (blurEffector_ != nullptr) {
		blurEffector_->Update();
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
