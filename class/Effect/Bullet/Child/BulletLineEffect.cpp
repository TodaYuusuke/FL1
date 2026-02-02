#include "BulletLineEffect.h"

using namespace LWP::Math;

BulletLineEffect::BulletLineEffect(LWP::Object::Camera* camera, const LWP::Math::Vector3& scale, LWP::Object::TransformQuat* transform, const LWP::Math::Vector3* vec)
{
    // 各種ポインタの受け取り
    camera_ = camera;
    bulletTransform_ = transform;
    bulletVector_ = vec;

    // 平面テクスチャ読み込み
    bulletLinePlane_.LoadTexture("Particle/Spark.png");
    // ベクトル設定
    bulletLinePlane_.velocity = *bulletVector_;

    // 座標追従
    bulletLinePlane_.worldTF.Parent(bulletTransform_);
    // 拡縮設定
    bulletLinePlane_.worldTF.scale = scale;
    bulletLinePlane_.worldTF.rotation = { 0.0f, 0.70710678f, 0.0f, 0.70710678f };
}

void BulletLineEffect::Update() {

}

LWP::Math::Quaternion BulletLineEffect::CalcPlaneAngle()
{
	// 弾、カメラの方向ベクトルを求める
	Vector3 dir = bulletVector_->Normalize();
	Vector3 view = camera_->worldTF.GetWorldPosition() - bulletTransform_->GetWorldPosition();
    view = view.Normalize();

	// 幅、上方方向のベクトルを求める
    Vector3 right = Vector3::Cross(view, dir);
    // 特異点に到達した場合
    if (right.Length() < 1e-6f) {
        right = Vector3::Cross({ 0.0f, 1.0f, 0.0f }, {0.0f, 1.0f, 0.0f}).Normalize();
    }
    right = right.Normalize();

	Vector3 up = Vector3::Cross(dir, right).Normalize();

	return LookRotation(dir, up);
}

LWP::Math::Quaternion BulletLineEffect::LookRotation(const LWP::Math::Vector3& forwardVec, const LWP::Math::Vector3& upVec)
{
	// 正面ベクトルを正規化
	Vector3 f = forwardVec.Normalize();

	// upをForwardに向かせる
	Vector3 r = Vector3::Cross(upVec, f).Normalize();

	// 特異点に到達した場合
	if (r.Length() < 1e-6f) {
		r = Vector3::Cross({ 0.0f, 1.0f, 0.0f }, f).Normalize();
	}
	Vector3 u = Vector3::Cross(f, r);

	// 回転クォータニオン生成
    return FromAxes(u, r, f);
}

LWP::Math::Quaternion BulletLineEffect::FromAxes(const LWP::Math::Vector3& r, const LWP::Math::Vector3& u, const LWP::Math::Vector3& f)
{
    float m00 = r.x, m01 = u.x, m02 = f.x;
    float m10 = r.y, m11 = u.y, m12 = f.y;
    float m20 = r.z, m21 = u.z, m22 = f.z;

    float trace = m00 + m11 + m22;

    Quaternion q;

    if (trace > 0.0f)
    {
        float s = std::sqrt(trace + 1.0f) * 2.0f;
        q.w = 0.25f * s;
        q.x = (m21 - m12) / s;
        q.y = (m02 - m20) / s;
        q.z = (m10 - m01) / s;
    }
    else if (m00 > m11 && m00 > m22)
    {
        float s = std::sqrt(1.0f + m00 - m11 - m22) * 2.0f;
        q.w = (m21 - m12) / s;
        q.x = 0.25f * s;
        q.y = (m01 + m10) / s;
        q.z = (m02 + m20) / s;
    }
    else if (m11 > m22)
    {
        float s = std::sqrt(1.0f + m11 - m00 - m22) * 2.0f;
        q.w = (m02 - m20) / s;
        q.x = (m01 + m10) / s;
        q.y = 0.25f * s;
        q.z = (m12 + m21) / s;
    }
    else
    {
        float s = std::sqrt(1.0f + m22 - m00 - m11) * 2.0f;
        q.w = (m10 - m01) / s;
        q.x = (m02 + m20) / s;
        q.y = (m12 + m21) / s;
        q.z = 0.25f * s;
    }

    return q.Normalize();
}


