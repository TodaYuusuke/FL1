#pragma once
#include <Adapter.h>

/// <summary>
/// エフェクトで使用するデータ構造体
/// </summary>
namespace LWP::Effect {

	// 振れ幅が必要なデータの構造体
	template<typename T>
	struct RandomData {
		T min{}; // 最小値
		T max{}; // 最大値

		/// <summary>
		/// 型によるランダムな値を取得する
		/// </summary>
		/// <returns>ランダムな値</returns>
		T Random() {
			// 返還用
			T result{};

			// 型によって処理変更
			if constexpr (std::is_same_v<T, LWP::Math::Vector3>) {
				result = LWP::Utility::Random::GenerateVector3(min, max);
			}
			else if constexpr (std::is_same_v<T, LWP::Math::Vector4>) {
				LWP::Math::Vector4 a = min;
				LWP::Math::Vector4 b = max;

				result = LWP::Math::Vector4(
					LWP::Utility::Random::GenerateFloat(a.x, b.x),
					LWP::Utility::Random::GenerateFloat(a.y, b.y),
					LWP::Utility::Random::GenerateFloat(a.z, b.z),
					LWP::Utility::Random::GenerateFloat(a.w, b.w)
				);
			}
			else if constexpr (std::is_same_v<T, LWP::Math::Vector2>) {
				result = LWP::Utility::Random::GenerateVector2(min, max);
			}
			else if constexpr (std::is_same_v<T, float>) {
				result = LWP::Utility::Random::GenerateFloat(min, max);
			}
			else if constexpr (std::is_same_v<T, int>) {
				result = LWP::Utility::Random::GenerateInt(min, max);
			}

			// 結果を返還
			return result;
		}
	};

	/// <summary>
	/// エフェクト汎用基底データ
	/// </summary>
	template <typename T>
	struct IData {
		bool			isUsed = false;		// 使用中フラグ	
		T				startValue{};		// 開始値
		RandomData<T>	startValueAmp{};	// 終了値

	};

	/// <summary>
	/// 速度データ
	/// </summary>
	template <typename T>
	struct VelocityData : public IData<T> {
		T				velocity{};			// 速度
		RandomData<T>	velocityAmp{};		// 速度の振れ幅
		T				acceleration{};		// 加速度
		RandomData<T>	accelerationAmp{};	// 加速度の振れ幅

		VelocityData<T> Convert() {
			VelocityData<T> result = *this;

			// ランダムな値を求めて返還する
			result.startValue += this->startValueAmp.Random();		// 開始値
			result.velocity += this->velocityAmp.Random();			// 速度
			result.acceleration += this->accelerationAmp.Random();	// 加速度

			return result;
		}
	};

	/// <summary>
	/// イージングデータ
	/// </summary>
	template <typename T>
	struct EasingData : public IData<T> {
		T				endValue{};												// 速度
		RandomData<T>	endValueAmp{};											// 速度の振れ幅
		LWP::Utility::Easing::Type type = LWP::Utility::Easing::Type::Liner;	// 補間タイプ

		EasingData<T> Convert() {
			EasingData<T> result = *this;

			// ランダムな値を求めて返還する
			result.startValue	+= this->startValueAmp.Random();	// 開始値
			result.endValue		+= this->endValueAmp.Random();		// 終了値
			return result;
		}
	};

}
