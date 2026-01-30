#pragma once

enum class CameraEffectType {
	kShake,
	kZoom,
	kBound,
	kCount
};

// 初期FOV
inline const float kDefaultFov = 180.0f;