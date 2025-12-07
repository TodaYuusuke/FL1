#pragma once
#include "../IParticleComponent.h"
#include "utility/motionEffects/IncludeEffects.h"
#include "../../../Utility/DeltaTimer.h"
#include "../../../Utility/Lerp.h"

/// <summary>
/// イージングコンポーネント
/// </summary>
template <typename T>
class EasingComponent : public IParticleComponent<T>
{
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="value">変更する値のアドレス</param>
	/// <param name="end">終了地点</param>
	/// <param name="easeTime">補間にかかる時間</param>
	/// <param name="type">補間タイプ</param>
	EasingComponent(T* value, const T& end, const float easeTime, LWP::Utility::Easing::Type type);
	
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(const float deltaTime, const float playSpeed) override;

private: // メンバ変数

	// 値のポインタ
	T* value_ = nullptr;

	// 開始地点
	T start_{};
	// 終了地点
	T end_{};

	// 補間秒数タイマー
	LWP::Utility::DeltaTimer easingTimer_{};
	// 補間秒数
	float easeTime_ = 0.0f;
	// 補間タイプ
	LWP::Utility::Easing::Type type_ = LWP::Utility::Easing::Type::Liner;
};

template<typename T>
inline EasingComponent<T>::EasingComponent(T* value, const T& end, const float easeTime, LWP::Utility::Easing::Type type)
{
	// 値のアドレスを渡す
	value_ = value;

	// それぞれの値取得
	start_		= *value_;
	end_		= end;
	easeTime_	= easeTime;
	type_		= type;
}

template<typename T>
inline void EasingComponent<T>::Init()
{
	// 補間開始
	easingTimer_.Start(easeTime_);
}

template<typename T>
inline void EasingComponent<T>::Update(const float deltaTime, const float playSpeed)
{
	// タイマーの再生速度の調整
	easingTimer_.SetTimerSpeed(playSpeed);

	if (!easingTimer_.GetIsFinish()) {
		// イージングで座標移動を行う
		*value_ = LWP::Utility::Lerp(start_, end_, easingTimer_.GetProgress());
	}
	else {
		// イージングで座標移動を行う
		*value_ = end_;
	}

	// イージングの更新
	easingTimer_.Update();

	// 警告回避
	deltaTime;
}
