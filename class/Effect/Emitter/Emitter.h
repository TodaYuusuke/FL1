#pragma once
#include "../Particle/Particle.h"

/// <summary>
/// パーティクル生成の管理クラス
/// </summary>
class Emitter
{
public: // サブクラス

	/// <summary>
	/// 粒子種類
	/// </summary>
	enum ParticleType {
		Surface,	// 平面
		model3D,	// 3Dモデル
		PTypeCount,	 // カウント用
	};

	/// <summary>
	/// 平面のタイプ
	/// </summary>
	enum SurfaceType {
		Normal,					// 通常
		Billboard,				// ビルボード
		HorizontalBillboard,	// 水平ビルボード
		VerticalBillboard,		// 垂直ビルボード
		StretchBillboard,		// ストレッチビルボード
		STypeCount,				// カウント用
	};

public: // コンストラクタ等

	// デフォルトコンストラクタの削除
	Emitter() = delete;

	/// <summary>
	/// コンストラクタ(平面パーティクル)
	/// </summary>
	/// <param name="texID">テクスチャID</param>
	/// <param name="SurfaceType">生成される平面のタイプ</param>
	/// <param name="pos">初期座標</param>
	Emitter(LWP::Resource::Texture texID, int surfaceType, const LWP::Math::Vector3& pos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Emitter();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="aliveTime">生存時間</param>
	/// <param name="emitTime">粒子生成にかかる秒数</param>
	/// <param name="emitCount">一度に生成される個数</param>
	/// <param name="maxEmitCount">最大生成個数</param>
	Emitter& Init(const float aliveTime, const float emitTime, const int32_t emitCount, const int32_t maxEmitCount);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(const float deltaTime, const float playSpeed);

public: // アクセッサ等

	/// <summary>
	/// 終了状態ゲッター
	/// </summary>
	/// <returns>エミッタの終了状態</returns>
	bool GetIsEnd() { return isEnd_; }

	/// <summary>
	/// 親のセッター
	/// </summary>
	/// <param name="parent">親</param>
	void SetParent(LWP::Object::TransformQuat* parent) { transform_.Parent(parent); }

	
	Emitter& SetIsWaitDeleteAllParticles(const bool isWait);

	/// <summary>
	/// 生成間隔秒数の振れ幅設定
	/// </summary>
	/// <param name="minTime">最小値</param>
	/// <param name="maxTime">最大値</param>
	Emitter& SetEmitTimeAmp(const float minTime, const float maxTime);

	/// <summary>
	/// 粒子生成秒数の振れ幅設定
	/// </summary>
	/// <param name="minTime">最小値</param>
	/// <param name="maxTime">最大値</param>
	Emitter& SetParticleAliveTimeAmp(const float minTime, const float maxTime);

	/// <summary>
	/// 粒子の移動速度設定
	/// </summary>
	/// <param name="data">データ本体</param>
	Emitter& SetTranslateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data);

	/// <summary>
	/// 粒子の移動補間設定
	/// </summary>
	/// <param name="data">データ本体</param>
	Emitter& SetTranslateEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data);
	
	/// <summary>
	/// 粒子の回転速度設定
	/// </summary>
	/// <param name="data">データ本体</param>
	Emitter& SetRotateVelocity(const LWP::Effect::VelocityData<LWP::Math::Vector3>& data);

	/// <summary>
	/// 粒子の拡縮イージング設定
	/// </summary>
	/// <param name="data">データ本体</param>
	Emitter& SetScaleEasing(const LWP::Effect::EasingData<LWP::Math::Vector3>& data);
	
	/// <summary>
	/// 粒子の色イージング設定
	/// </summary>
	/// <param name="data">データ本体</param>
	Emitter& SetColorEasing(const LWP::Effect::EasingData<LWP::Math::Vector4>& data);

private: // プライベートなメンバ関数

	/// <summary>
	/// 粒子生成関数
	/// </summary>
	void Emit();

	/// <summary>
	/// 平面生成時の処理関数
	/// </summary>
	void EmitSurface();

public: // パブリックなメンバ関数

	// エミッタ自体のワールドトランスフォーム
	LWP::Object::TransformQuat transform_;

private: // メンバ変数

	// 粒子のタイプ
	int particleType_ = Emitter::ParticleType::Surface;

	// 平面描画時のタイプ
	int surfaceType_ = Emitter::SurfaceType::Normal;
	// 平面描画の際に使用するテクスチャID
	LWP::Resource::Texture tex_;

	// 終了時間タイマー
	LWP::Utility::DeltaTimer aliveTimer_{};
	// 終了時間
	float aliveTime_ = 5.0f;
	// 終了フラグ
	bool isEnd_ = false;

	// 粒子生成の生成時間タイマー
	LWP::Utility::DeltaTimer emitTimer_{};
	// 粒子の生成時間
	float emitTime_ = 0.1f;
	// 粒子の生成時間振れ幅
	LWP::Effect::RandomData<float> emitTimeAmp_{};
	// 粒子の生成フラグ
	bool isEmit_ = false;

	// 生成上限数
	int32_t maxEmitCount_ = 100;
	// 一度に生成される個数
	int32_t emitCount_ = 5;

	// 粒子の格納配列
	std::list<Particle*> particles_{};

	// 粒子の生成時間
	LWP::Effect::RandomData<float> pAliveTime_{};
	// 粒子データ系
	LWP::Effect::VelocityData<LWP::Math::Vector3>	pVelocityPos_{};
	LWP::Effect::EasingData<LWP::Math::Vector3>		pEasingPos_{};
	LWP::Effect::VelocityData<LWP::Math::Vector3>	pVelocityRotate_{};
	LWP::Effect::EasingData<LWP::Math::Vector3>		pEasingScale_{};
	LWP::Effect::EasingData<LWP::Math::Vector4>		pEasingColor_{};

	// エミッタ終了時、粒子がすべて消えるまで待機するか
	bool isWaitDeleteAllParticles_ = false;
	// 拡縮の際、アスペクト比を統一するか
	bool unificationRandomScale_ = false;
};

