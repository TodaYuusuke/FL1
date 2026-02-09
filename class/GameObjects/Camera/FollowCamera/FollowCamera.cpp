#include "FollowCamera.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/Input/VirtualController.h"
#include "../CameraConfig.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility::Interpolation;

FollowCamera::FollowCamera(LWP::Object::Camera* camera) {
	camera_ = camera;
	debugCamera_ = camera;

	// ポストプロセス有効
	camera_->pp.use = true;
	camera_->pp.bloom.use = true;
	camera_->pp.radialBlur.use = true;
	camera_->pp.rgbShift.use = true;
	camera_->pp.vignetting.use = true;
	camera_->pp.fog.use = true;
	camera_->pp.CreateShaderFile();

	camera_->pp.fog.fogNear = 150.0f;
	camera_->pp.fog.fogFar = 735.0f;
	camera_->pp.fog.color = Utility::Color(0x573923FF);

	camera->farClip = 600.0f;

	Init();
}

void FollowCamera::Init() {
	kTargetDist = defaultTargetDist_;

	// x軸回転
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, LWP::Utility::DegreeToRadian(kStartAngle.x));
	// y軸は常に上を向くように固定
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, LWP::Utility::DegreeToRadian(kStartAngle.y)) * camera_->worldTF.rotation;

	// FOVの初期値を設定
	camera_->fov = kDefaultFov;
}

void FollowCamera::Update() {
	// カメラの座標を決定
	camera_->worldTF.translation = kTargetDist;
}

void FollowCamera::DebugGUI() {
	// 追従対象がいないときのみデバッグカメラ使用可能
	if (!target_) {
		if (ImGui::TreeNode("DebugCamera")) {
			debugCamera_->DebugGUI();
			ImGui::TreePop();
		}
	}

	ImGui::DragFloat3("Translation", &camera_->worldTF.translation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat4("Quaternion", &camera_->worldTF.rotation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat3("Distance", &kTargetDist.x, 0.1f, -100, 100);
	ImGui::DragFloat("FOV", &camera_->fov, 0.1f);
}

void FollowCamera::SetTarget(Actor* actor) {
	target_ = actor;
	camera_->worldTF.Parent(target_->GetWorldTF());
}