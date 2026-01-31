#pragma once
#include "../IBulletEffect.h"

/// <summary>
/// 弾道エフェクト単体
/// </summary>
class BulletLineEffect : public IBulletEffect
{
public: // コンストラクタ

	// デフォルトコンストラクタ削除
	BulletLineEffect() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="camera">使用中のカメラ</param>
	/// <param name="scale">大きさ</param>
	/// <param name="transform">弾の座標</param>
	/// <param name="vec">弾のベクトル</param>
	BulletLineEffect(LWP::Object::Camera* camera, const LWP::Math::Vector3& scale, LWP::Object::TransformQuat* transform, const LWP::Math::Vector3* vec);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BulletLineEffect() = default;

public: // メンバ関数

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private: // プライベートなメンバ関数

	/// <summary>
	/// ストレッチビルボード平面角度の計算関数
	/// </summary>
	/// <returns>平面角度</returns>
	LWP::Math::Quaternion CalcPlaneAngle();

	/// <summary>
	/// 引数で指定されたベクトルの方向に向かせる関数
	/// </summary>
	/// <param name="forwardVec">前方向ベクトル</param>
	/// <param name="upVec">上方向ベクトル</param>
	/// <returns>計算後のクォータニオン</returns>
	LWP::Math::Quaternion LookRotation(const LWP::Math::Vector3& forwardVec, const LWP::Math::Vector3& upVec);

	/// <summary>
	/// 基底からクォータニオンを生成する関数
	/// </summary>
	/// <param name="r"></param>
	/// <param name="u"></param>
	/// <param name="f"></param>
	/// <returns>クォータニオン</returns>
	LWP::Math::Quaternion FromAxes(const LWP::Math::Vector3& r, const LWP::Math::Vector3& u, const LWP::Math::Vector3& f);

private: // メンバ変数

	// ライン用平面
	LWP::Primitive::NormalStretchedBillboard bulletLinePlane_{};

	// カメラのポインタ
	LWP::Object::Camera* camera_ = nullptr;

	// 弾のトランスフォーム
	LWP::Object::TransformQuat* bulletTransform_ = nullptr;
	// 弾のベクトルポインタ
	const LWP::Math::Vector3* bulletVector_ = nullptr;

};
