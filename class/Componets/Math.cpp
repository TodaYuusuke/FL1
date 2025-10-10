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

	LWP::Math::Vector3 Abs(LWP::Math::Vector3 value) {
		LWP::Math::Vector3 result{
			std::fabsf(value.x),
			std::fabsf(value.y),
			std::fabsf(value.z)
		};
		return result;
	}
}