#pragma once
#include "../IParticle.h"

/// <summary>
/// 連番アニメーションパーティクル
/// </summary>
class SequenceParticle : public IParticle
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	SequenceParticle() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="plane">平面</param>
	/// <param name="index">連番アニメーションインデックスのアドレス</param>
	/// <param name="splitSize">連番一枚ごとの分割サイズ</param>
	/// <param name="animTime">アニメーション時間(秒)</param>
	/// <param name="isLoop">ループするか</param>
	/// <param name="isStretchBillboard">ストレッチビルボードかどうか</param>
	SequenceParticle(std::unique_ptr<LWP::Primitive::IPlane> plane, int* index, const LWP::Math::Vector2& splitSize, const float animTime, const bool isLoop, bool isStretchBillboard);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SequenceParticle();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	SequenceParticle& Init() override;

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
	SequenceParticle& SetParent(LWP::Object::TransformQuat* parent) override;

	/// <summary>
	/// トランスフォームの設定
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転</param>
	/// <param name="translate">座標</param>
	SequenceParticle& SetTransform(const LWP::Math::Vector3& scale, const LWP::Math::Quaternion& rotate, const LWP::Math::Vector3& translate) override;

	/// <summary>
	/// 生存時間設定
	/// </summary>
	/// <param name="aliveTime">生存時間</param>
	SequenceParticle& SetAliveTime(const float aliveTime) override;

	/// <summary>
	/// 移動速度設定
	/// </summary>
	SequenceParticle& SetVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data) override;

	/// <summary>
	/// 移動補間設定
	/// </summary>
	SequenceParticle& SetEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data) override;

	/// <summary>
	/// 回転速度設定
	/// </summary>
	SequenceParticle& SetRotateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data) override;

	/// <summary>
	/// 拡縮イージング設定
	/// </summary>
	SequenceParticle& SetScaleEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data, const bool isUnificationScale) override;

	/// <summary>
	/// 色イージング設定
	/// </summary>
	SequenceParticle& SetColorEasing(const LWP::Effect::EasingData<LWP::Math::Vector4>& data) override;

private: // プライベートなメンバ関数

	/// <summary>
	/// 連番アニメーション更新関数
	/// </summary>
	void SequenceUpdate();

private: // メンバ変数

	// 平面
	std::unique_ptr<LWP::Primitive::IPlane> plane_ = nullptr;

	// 連番アニメーション用タイマー
	LWP::Utility::DeltaTimer animTimer_{};
	// 連番アニメーションの再生秒数
	float animTime_ = 0.0f;
	// 連番アニメーションのループフラグ
	bool isAnimLoop_ = false;
	// 連番アニメーション終了フラグ
	bool isAnimEnd_ = false;

	// 最大分割数
	int splitNum_ = 0;

	// 連番アニメーションのインデックアドレス
	int* sequeceIndex_ = nullptr;

	// ストレッチビルボードのベクトルアドレス格納用
	LWP::Math::Vector3* stretchVector_ = nullptr;

};

