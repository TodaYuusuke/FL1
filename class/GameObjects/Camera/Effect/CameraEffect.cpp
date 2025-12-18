#include "CameraEffect.h"

CameraEffect::CameraEffect(FollowCamera* camera) {
	pCamera_ = camera;
	isActive_ = true;
}
