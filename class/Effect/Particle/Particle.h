#pragma once
#include "Components/ParticleSystemIncluder.h"
#include "../EffectStructs.h"

/// <summary>
/// パーティクルクラス
/// </summary>
class Particle
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	Particle() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Particle(std::unique_ptr<LWP::Primitive::IPlane> plane, bool isStretchBillboard = false);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particle();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="plane">平面のポインタ</param>
	Particle& Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	/// <param name="playSpeed">再生スピード</param>
	void Update(const float deltaTime, const float playSpeed);

public: // アクセッサ等

	/// <summary>
	/// 終了状態ゲッター
	/// </summary>
	/// <returns>終了状態</returns>
	bool GetIsEnd() { return isEnd_; }

	/// <summary>
	/// 粒子の親子付け設定
	/// </summary>
	/// <param name="parent">粒子の親となるエミッタ</param>
	Particle& SetParent(LWP::Object::TransformQuat* parent);

	/// <summary>
	/// トランスフォームの設定
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転</param>
	/// <param name="translate">座標</param>
	Particle& SetTransform(const LWP::Math::Vector3& scale, const LWP::Math::Quaternion& rotate, const LWP::Math::Vector3& translate);

	/// <summary>
	/// 生存時間設定
	/// </summary>
	/// <param name="aliveTime">生存時間</param>
	Particle& SetAliveTime(const float aliveTime);

	/// <summary>
	/// 移動速度設定
	/// </summary>
	Particle& SetVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data);

	/// <summary>
	/// 移動補間設定
	/// </summary>
	Particle& SetEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data);

	/// <summary>
	/// 回転速度設定
	/// </summary>
	Particle& SetRotateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data);

	/// <summary>
	/// 拡縮イージング設定
	/// </summary>
	Particle& SetScaleEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data, const bool isUnificationScale);

	/// <summary>
	/// 色イージング設定
	/// </summary>
	Particle& SetColorEasing(const LWP::Effect::EasingData<LWP::Math::Vector4>& data);

private: // メンバ変数

	// 平面
	std::unique_ptr<LWP::Primitive::IPlane> plane_ = nullptr;

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

	// ストレッチビルボードのベクトルアドレス格納用
	LWP::Math::Vector3* stretchVector_ = nullptr;
};

