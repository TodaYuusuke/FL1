#pragma once
#include <Adapter.h>

namespace FLMath {
	/// <summary>
	/// 方向ベクトルからクォータニオンを算出
	/// </summary>
	/// <param name="dirVec"></param>
	/// <returns></returns>
	LWP::Math::Quaternion LookRotation(const LWP::Math::Vector3& dirVec);

	float LerpShortAngle(float a, float b, float t);

	/// <summary>
	/// 角度から方向ベクトルを算出
	/// </summary>
	/// <param name="dirVel"></param>
	/// <param name="q"></param>
	/// <returns></returns>
	LWP::Math::Vector3 GetDirVector(const LWP::Math::Vector3& dirVel, const LWP::Math::Quaternion& q);
	LWP::Math::Vector3 GetDirVector(const LWP::Math::Vector3& dirVel, const LWP::Math::Vector3& radian);

	/// <summary>
	/// 絶対値に変換
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	LWP::Math::Vector3 Abs(LWP::Math::Vector3 value);
}