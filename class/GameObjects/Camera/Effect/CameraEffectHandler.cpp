#include "CameraEffectHandler.h"
#include "../../../Componets/Math.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace FLMath;

CameraEffectHandler::CameraEffectHandler() {

}

void CameraEffectHandler::Init() {
	for (auto& effect : cameraEffects_) {
		effect.second->Update();
	}
}

void CameraEffectHandler::Update() {
	for (auto& effect : cameraEffects_) {
		effect.second->Update();
	}
}

void CameraEffectHandler::StartShake(LWP::Math::Vector3 range, float endTime) {
	// 生成されていたら削除
	if (cameraEffects_[CameraEffectType::kShake]) { cameraEffects_[CameraEffectType::kShake].reset(); }

	// 生成
	cameraEffects_[CameraEffectType::kShake] = std::make_unique<CameraShake>(effectTarget_, range, endTime);
}

void CameraEffectHandler::StartZoom(float zoomValue, float endTime) {
	// 生成されていたら削除
	if (cameraEffects_[CameraEffectType::kZoom]) { cameraEffects_[CameraEffectType::kZoom].reset(); }

	// 生成
	cameraEffects_[CameraEffectType::kZoom] = std::make_unique<CameraZoom>(effectTarget_, zoomValue, endTime);
}
