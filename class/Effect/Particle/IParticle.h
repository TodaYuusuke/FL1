#pragma once
#include "Components/ParticleSystemIncluder.h"
#include "../EffectStructs.h"

/// <summary>
/// パーティクル基底クラス
/// </summary>
class IParticle
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	IParticle() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IParticle() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="plane">平面のポインタ</param>
	virtual IParticle& Init() = 0;

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	/// <param name="playSpeed">再生スピード</param>
	virtual void Update(const float deltaTime, const float playSpeed) = 0;

public: // アクセッサ等

	/// <summary>
	/// 終了状態ゲッター
	/// </summary>
	/// <returns>終了状態</returns>
	virtual bool GetIsEnd() = 0;

	/// <summary>
	/// 粒子の親子付け設定
	/// </summary>
	/// <param name="parent">粒子の親となるエミッタ</param>
	virtual IParticle& SetParent(LWP::Object::TransformQuat* parent) = 0;

	/// <summary>
	/// トランスフォームの設定
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転</param>
	/// <param name="translate">座標</param>
	virtual IParticle& SetTransform(const LWP::Math::Vector3& scale, const LWP::Math::Quaternion& rotate, const LWP::Math::Vector3& translate) = 0;

	/// <summary>
	/// 生存時間設定
	/// </summary>
	/// <param name="aliveTime">生存時間</param>
	virtual IParticle& SetAliveTime(const float aliveTime) = 0;

	/// <summary>
	/// 移動速度設定
	/// </summary>
	virtual IParticle& SetVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data) = 0;

	/// <summary>
	/// 移動補間設定
	/// </summary>
	virtual IParticle& SetEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data) = 0;

	/// <summary>
	/// 回転速度設定
	/// </summary>
	virtual IParticle& SetRotateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data) = 0;

	/// <summary>
	/// 拡縮イージング設定
	/// </summary>
	virtual IParticle& SetScaleEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data, const bool isUnificationScale) = 0;

	/// <summary>
	/// 色イージング設定
	/// </summary>
	virtual IParticle& SetColorEasing(const LWP::Effect::EasingData<LWP::Math::Vector4>& data) = 0;

protected: // メンバ変数

	// 終了フラグ
	bool isEnd_ = false;

	// 粒子の生存時間タイマー
	LWP::Utility::DeltaTimer aliveTimer_;
	// 粒子の生存時間
	float aliveTime_{};

	// 粒子の色
	LWP::Math::Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 移動用コンポーネント
	IParticleComponent<LWP::Math::Vector3>* moveComponent_ = nullptr;
	
	// 回転用コンポーネント
	IParticleComponent<LWP::Math::Quaternion>* rotateComponent_ = nullptr;
	
	// 拡縮用コンポーネント
	IParticleComponent<LWP::Math::Vector3>* scaleComponent_ = nullptr;
	
	// 色用コンポーネント
	IParticleComponent<LWP::Math::Vector4>* colorComponent_ = nullptr;
};

