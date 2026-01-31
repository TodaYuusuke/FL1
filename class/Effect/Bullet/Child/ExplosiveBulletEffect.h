#pragma once
#include "../IBulletEffect.h"
#include "../../EffectManager.h"

/// <summary>
/// 爆発弾エフェクト
/// </summary>
class ExplosiveBulletEffect : public IBulletEffect
{
public: // コンストラクタ

	// デフォルトコンストラクタ削除
	ExplosiveBulletEffect() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="scale">大きさ</param>
	/// <param name="transform">弾の座標</param>
	ExplosiveBulletEffect(const LWP::Math::Vector3& scale, LWP::Object::TransformQuat* transform);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ExplosiveBulletEffect();

public: // メンバ関数

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private: // メンバ変数

	// 弾用平面
	LWP::Primitive::NormalBillboard2D bulletPlane_{};

	// 煙エフェクトエミッタ
	Emitter* smokeEffectEmitter_ = nullptr;

	// 弾のトランスフォーム
	LWP::Object::TransformQuat* bulletTransform_ = nullptr;

};

