#pragma once
#include "../IParticle.h"

/// <summary>
/// 3Dモデルパーティクルクラス
/// </summary>
class ModelParticle : public IParticle
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	ModelParticle() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelParticle(std::unique_ptr<LWP::Resource::RigidModel> model);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelParticle();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	ModelParticle& Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	/// <param name="playSpeed">再生スピード</param>
	void Update(const float deltaTime, const float playSpeed) override;

public: // アクセッサ等

	/// <summary>
	/// 終了状態ゲッター
	/// </summary>
	/// <returns>終了状態</returns>
	bool GetIsEnd() override { return isEnd_; }

	/// <summary>
	/// 粒子の親子付け設定
	/// </summary>
	/// <param name="parent">粒子の親となるエミッタ</param>
	ModelParticle& SetParent(LWP::Object::TransformQuat* parent) override;

	/// <summary>
	/// トランスフォームの設定
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転</param>
	/// <param name="translate">座標</param>
	ModelParticle& SetTransform(const LWP::Math::Vector3& scale, const LWP::Math::Quaternion& rotate, const LWP::Math::Vector3& translate) override;

	/// <summary>
	/// 生存時間設定
	/// </summary>
	/// <param name="aliveTime">生存時間</param>
	ModelParticle& SetAliveTime(const float aliveTime) override;

	/// <summary>
	/// 移動速度設定
	/// </summary>
	ModelParticle& SetVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data) override;

	/// <summary>
	/// 移動補間設定
	/// </summary>
	ModelParticle& SetEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data) override;

	/// <summary>
	/// 回転速度設定
	/// </summary>
	ModelParticle& SetRotateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data) override;

	/// <summary>
	/// 拡縮イージング設定
	/// </summary>
	ModelParticle& SetScaleEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data, const bool isUnificationScale) override;

	/// <summary>
	/// 色イージング設定
	/// </summary>
	ModelParticle& SetColorEasing(const LWP::Effect::EasingData<LWP::Math::Vector4>& data) override;

private: // メンバ変数

	// 平面
	std::unique_ptr<LWP::Resource::RigidModel> model_ = nullptr;
};

