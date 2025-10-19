#include "Math.h"
#include <cmath>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Utility::Interpolation;

namespace FLMath {
	Quaternion LookRotation(const Vector3& dirVec) {
		// 向かせたい方向ベクトル
		Vector3 dir = dirVec.Normalize();
		// デフォルトの方向ベクトル
		Vector3 norm = { 0,0,1 };
		// 二つの方向ベクトルから角度算出
		float theta = std::acosf(Vector3::Dot(norm, dir)) / 2.0f;
		Vector3 cross = Vector3::Cross(norm, dir).Normalize();

		// 計算結果
		Quaternion q = {
			cross.x * std::sin(theta),
			cross.y * std::sin(theta),
			cross.z * std::sin(theta),
			std::cos(theta)
		};
		return q;
	}

	float LerpShortAngle(float a, float b, float t) {
		// 角度差分を求める
		float diff = b - a;

		float M_PI = 3.14f;

		diff = std::fmod(diff, 2 * (float)M_PI);
		if (diff < 2 * (float)-M_PI) {
			diff += 2 * (float)M_PI;
		}
		else if (diff >= 2 * M_PI) {
			diff -= 2 * (float)M_PI;
		}

		diff = std::fmod(diff, 2 * (float)M_PI);
		if (diff < (float)-M_PI) {
			diff += 2 * (float)M_PI;
		}
		else if (diff >= (float)M_PI) {
			diff -= 2 * (float)M_PI;
		}

		return LWP::Utility::Interpolation::LerpF(a, diff, t);
	}

	LWP::Math::Vector3 GetDirVector(const LWP::Math::Vector3& dirVel, const LWP::Math::Quaternion& q) {
		Vector3 result = dirVel * Matrix4x4::CreateRotateXYZMatrix(q);
		return result.Normalize();
	}
	LWP::Math::Vector3 GetDirVector(const LWP::Math::Vector3& dirVel, const LWP::Math::Vector3& radian) {
		Vector3 result = dirVel * Matrix4x4::CreateRotateXYZMatrix(radian);
		return result.Normalize();
	}

	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
		Vector3 result{};
		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
			1.0f * matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
			1.0f * matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
			1.0f * matrix.m[3][2];
		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
			1.0f * matrix.m[3][3];
		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		result.z /= w;
		return result;
	}

	Vector2 ConvertWorldToScreen(const Vector3& worldPos, const Matrix4x4& viewProjection) {
		Vector3 result = worldPos;
		if (result.x == 0 && result.y == 0 && result.z == 0) {
			result = { 0.000001f,0.000001f ,0.0f };
		}
		// ビューポート行列
		Matrix4x4 matViewport = Matrix4x4::CreateViewportMatrix(0, 0, LWP::Info::GetWindowWidthF(), LWP::Info::GetWindowHeightF(), 0, 1);
		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport = viewProjection * matViewport;
		// ワールド→スクリーン座標変換
		result = Transform(result, matViewProjectionViewport);

		return Vector2(result.x, result.y);
	}

	bool IsObjectInFront(const Vector3& objectPos, const Vector3& cameraPos, const Quaternion& cameraRotate) {
		// カメラの角度方向ベクトルに変換
		Vector3 offset{ 0, 0, 1 };
		// 回転行列を合成
		Matrix4x4 rotateMatrix = Matrix4x4::CreateRotateXYZMatrix(cameraRotate);
		// 自機のワールド行列の回転を反映する
		offset = offset * rotateMatrix;

		// 自機と敵の方向ベクトルを算出
		Vector3 p2eDirVel = (objectPos - cameraPos).Normalize();

		float dotXZ = Vector2::Dot(Vector2{ offset.x,offset.z }, Vector2{ p2eDirVel.x,p2eDirVel.z });
		float magnitude1XZ = Vector2{ offset.x,offset.z }.Length();
		float magnitude2XZ = Vector2{ p2eDirVel.x,p2eDirVel.z }.Length();
		float angleXZ = std::acos(dotXZ / (magnitude1XZ * magnitude2XZ));
		angleXZ = LWP::Utility::RadianToDegree(angleXZ);

		if ((angleXZ) < (90.0f)) {
			return true;
		}
		// カメラの映らないところにいる
		return false;
	}

	LWP::Math::Vector3 Abs(LWP::Math::Vector3 value) {
		LWP::Math::Vector3 result{
			std::fabsf(value.x),
			std::fabsf(value.y),
			std::fabsf(value.z)
		};
		return result;
	}
}