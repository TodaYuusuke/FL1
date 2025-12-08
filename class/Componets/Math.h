#pragma once
#include <Adapter.h>

namespace FLMath {
	/// <summary>
	/// 方向ベクトルからクォータニオンを算出
	/// </summary>
	/// <param name="dirVec"></param>
	/// <returns></returns>
	LWP::Math::Quaternion LookRotation(const LWP::Math::Vector3& dirVec);
	/// <summary>
	/// 方向ベクトルからクォータニオンを算出
	/// Z軸のみ計算しない
	/// </summary>
	/// <param name="dirVec"></param>
	/// <returns></returns>
	LWP::Math::Quaternion LookRotationZLock(const LWP::Math::Vector3& dirVec);

	LWP::Math::Quaternion AngularVelocityToQuaternion(const LWP::Math::Vector3& omega, float dt);

	float LerpShortAngle(float a, float b, float t);

	/// <summary>
	/// 角度から方向ベクトルを算出
	/// </summary>
	/// <param name="dirVel"></param>
	/// <param name="q"></param>
	/// <returns></returns>
	LWP::Math::Vector3 GetDirVector(const LWP::Math::Vector3& dirVel, const LWP::Math::Quaternion& q);
	LWP::Math::Vector3 GetDirVector(const LWP::Math::Vector3& dirVel, const LWP::Math::Vector3& radian);

	LWP::Math::Vector3 Transform(const LWP::Math::Vector3& vector, const LWP::Math::Matrix4x4& matrix);

	/// <summary>
	/// ワールド座標からスクリーン座標に変換
	/// </summary>
	/// <param name="worldPos"></param>
	/// <param name="viewProjection"></param>
	/// <returns></returns>
	LWP::Math::Vector2 ConvertWorldToScreen(const LWP::Math::Vector3& worldPos, const LWP::Math::Matrix4x4& viewProjection);
	/// <summary>
	/// スクリーン座標からワールド座標に変換
	/// </summary>
	/// <param name="vpMatrix">viewProjectionMatrix</param>
	/// <param name="screenPos">スクリーン座標</param>
	/// <param name="cameraDistance">カメラとの距離</param>
	LWP::Math::Vector3 ConvertScreenToWorld(const LWP::Math::Matrix4x4& vpMatrix, const LWP::Math::Vector2& screenPos, float cameraDistance);

	/// <summary>
	/// 対象がカメラの正面方向にいるか
	/// </summary>
	/// <param name="objectPos"></param>
	/// <returns></returns>
	bool IsObjectInFront(const LWP::Math::Vector3& objectPos, const LWP::Math::Vector3& cameraPos, const LWP::Math::Quaternion& cameraRotate);

	/// <summary>
	/// 絶対値に変換
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	LWP::Math::Vector3 Abs(LWP::Math::Vector3 value);

	float LengthSq(const LWP::Math::Vector3& value);
}