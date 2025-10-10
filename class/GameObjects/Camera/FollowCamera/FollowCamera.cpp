#include "FollowCamera.h"
#include "../../ICharacter.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility::Interpolation;

FollowCamera::FollowCamera(LWP::Object::Camera* camera) {
	camera_ = camera;

	Init();
}

void FollowCamera::Init() {
	kTargetDist = defaultTargetDist_;

	radian_ = {
		LWP::Utility::DegreeToRadian(kStartAngle.x),
		LWP::Utility::DegreeToRadian(kStartAngle.y)
	};

	// x軸回転
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, LWP::Utility::DegreeToRadian(kStartAngle.x));
	// y軸は常に上を向くように固定
	camera_->worldTF.rotation = LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, LWP::Utility::DegreeToRadian(kStartAngle.y)) * camera_->worldTF.rotation;
}

void FollowCamera::Update() {
	// 追従対象がいなければ処理しない
	if (!target_) { return; }

	//// 入力
	//InputHandle();

	//// x軸回転
	//SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 0, 1 }, radian_.z) * LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 1, 0, 0 }, radian_.x));
	//// y軸は常に上を向くように固定
	//SetCameraRotate(LWP::Math::Quaternion::CreateFromAxisAngle(LWP::Math::Vector3{ 0, 1, 0 }, radian_.y) * camera_->worldTF.rotation);


	camera_->worldTF.rotation = target_->GetWorldTF()->rotation;

	// 座標の補間をしていない座標を算出
	defaultPos_ = (target_->GetWorldTF()->GetWorldPosition()) + kTargetDist * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(camera_->worldTF.rotation);
	// カメラの後追い
	interTarget_ = LWP::Utility::Interpolation::Lerp(interTarget_, target_->GetWorldTF()->GetWorldPosition(), interTargetRate);
	// カメラの座標を決定
	camera_->worldTF.translation = interTarget_ + (kTargetDist * LWP::Math::Matrix4x4::CreateRotateXYZMatrix(camera_->worldTF.rotation));
}

void FollowCamera::DebugGUI() {
	ImGui::DragFloat3("Translation", &camera_->worldTF.translation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat4("Quaternion", &camera_->worldTF.rotation.x, 0.1f, -1000, 1000);
	ImGui::DragFloat3("Radian", &radian_.x, 0.1f, -1000, 1000);
	ImGui::DragFloat3("Distance", &kTargetDist.x, 0.1f, -100, 100);
}

void FollowCamera::InputHandle() {
	Vector2 dir{};

	// コントローラーでの回転
	dir.x -= LWP::Input::Pad::GetRStick().y * sensitivity;
	dir.y += LWP::Input::Pad::GetRStick().x * sensitivity;

	// スティックの入力をイージング
	LWP::Math::Vector3 goal = { dir.x, dir.y, 0 };
	stickDir_ = LWP::Utility::Interpolation::Exponential(stickDir_, goal, rotateRate);

	// 角度制限
	ClampAngle(stickDir_.x, (target_->GetWorldTF()->GetWorldPosition() - camera_->worldTF.translation).Normalize(), LWP::Utility::DegreeToRadian(kOriginRotateX + kMinRotateX), LWP::Utility::DegreeToRadian(kOriginRotateX + kMaxRotateX));

	radian_.x += 0.03f * stickDir_.x;
	radian_.y += 0.03f * stickDir_.y;

	if (radian_.y >= 2 * (float)std::numbers::pi) {
		radian_.y -= 2 * (float)std::numbers::pi;
	}
	if (radian_.y <= -2 * (float)std::numbers::pi) {
		radian_.y += 2 * (float)std::numbers::pi;
	}
	// Z軸を0に戻す
	radian_ = LWP::Utility::Interpolation::Exponential(radian_, LWP::Math::Vector3{ radian_.x, radian_.y, 0.0f }, 0.1f);
}

void FollowCamera::ClampAngle(float& target, LWP::Math::Vector3 distance, float minLimitAngle, float maxLimitAngle) {
	// ターゲットとカメラの角度を求める
	float limitX = std::acos(LWP::Math::Vector3::Dot({ 0,1,0 }, distance));
	// 下
	if (limitX < minLimitAngle && target <= 0.0f) {
		target = 0;
	}
	// 上
	if (limitX > maxLimitAngle && target >= 0.0f) {
		target = 0;
	}
}
