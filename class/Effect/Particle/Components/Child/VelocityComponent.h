#pragma once
#include "../IParticleComponent.h"

/// <summary>
/// 速度コンポーネント
/// </summary>
template <typename T>
class VelocityComponent : public IParticleComponent<T>
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="value">変更する値のアドレス</param>
	/// <param name="velocity">速度</param>
	/// <param name="acceleration">加速度</param>
	/// <param name="stretchVector">(任意) ストレッチビルボード用ベクトル</param>
	VelocityComponent(T* value, const LWP::Math::Vector3& velocity, const LWP::Math::Vector3& acceleration, LWP::Math::Vector3* stretchVector = nullptr);

	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(const float deltaTime, const float playSpeed) override;

public: // 機能系メンバ関数

	/// <summary>
	/// 度数法角度をラジアンに変換する
	/// </summary>
	/// <param name="degree">度</param>
	/// <returns>ラジアン</returns>
	LWP::Math::Vector3 DegreeToRad(const LWP::Math::Vector3 degree);

private: // メンバ変数

	// 値のポインタ
	T* value_ = nullptr;

	// 速度
	LWP::Math::Vector3 velocity_{};
	// 加速度
	LWP::Math::Vector3 acceleration_{};

	// ストレッチビルボード用ベクトルのアドレス格納用
	LWP::Math::Vector3* stretchVector_ = nullptr;
};

template<typename T>
inline VelocityComponent<T>::VelocityComponent(T* value, const LWP::Math::Vector3& velocity, const LWP::Math::Vector3& acceleration, LWP::Math::Vector3* stretchVector)
{
	// 座標アドレスを取得
	value_ = value;

	// 速度、加速度の受け取り
	velocity_ = velocity;
	acceleration_ = acceleration;

	// ストレッチビルボード用ベクトルの取得
	stretchVector_ = stretchVector;
}

template<typename T>
inline void VelocityComponent<T>::Init() { }

template<typename T>
inline void VelocityComponent<T>::Update(const float deltaTime, const float playSpeed)
{
	//クォータニオン型のみ処理を変更
	if constexpr (std::is_same_v<T, LWP::Math::Quaternion>) {
		// 1フレームごとの回転量を求める
		LWP::Math::Vector3 frameRotationAmount = velocity_ * deltaTime * playSpeed;
		// 求めた回転量をラジアンに変換する
		LWP::Math::Vector3 radRotationAmount = DegreeToRad(frameRotationAmount);

		// クォータニオンの回転量を求める
		LWP::Math::Quaternion rotationAmount = LWP::Math::Quaternion::ConvertEuler(radRotationAmount);
		// 回転量を加算する
		*value_ = *value_ * rotationAmount;
	}
	else {
		// 速度に加速度を加算
		velocity_ += (acceleration_ * playSpeed);

		// 速度を元に加算を行う
		*value_ = *value_ + (velocity_ * playSpeed);

		// ストレッチビルボード用ベクトルのポインタが渡されている場合
		if (stretchVector_ != nullptr) {
			// 正規化したベクトルをストレッチビルボードに渡す
			*stretchVector_ = velocity_.Normalize();
		}
	}
}

template<typename T>
inline LWP::Math::Vector3 VelocityComponent<T>::DegreeToRad(const LWP::Math::Vector3 degree)
{
	return LWP::Math::Vector3(
		LWP::Utility::DegreeToRadian(degree.x),
		LWP::Utility::DegreeToRadian(degree.y),
		LWP::Utility::DegreeToRadian(degree.z)
	
	);
}
