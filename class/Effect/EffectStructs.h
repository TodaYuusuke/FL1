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
		int eType = 0;															// 保存用
		
		EasingData<T> Convert() {
			this->type = static_cast<LWP::Utility::Easing::Type>(eType); // 補間種類の設定
			EasingData<T> result = *this;

			// ランダムな値を求めて返還する
			result.startValue	+= this->startValueAmp.Random();	// 開始値
			result.endValue		+= this->endValueAmp.Random();		// 終了値
			return result;
		}
	};

	/// <summary>
	/// パーティクル保存用構造体
	/// </summary>
	struct EffectSaveData {
		int ParticleType = 0;
		std::string ModelPath = "cube.obj";
		int SurfaceType = 0;
		std::string TexPath = "circle.png";
		LWP::Math::Vector2 SplitSize{};
		float AnimTime = 0.0f;
		bool IsAnimLoop = false;
		float EmitAliveTime = 5.0f;
		bool IsWaitDeleteAllParticles = false;
		int EmitCount = 3;
		int MaxEmitCount = 100;
		float EmitTime = 0.1f;
		bool unificationRandomScale = false;
		RandomData<float>					EmitTimeAmp{};
		RandomData<float>					AliveTimeAmp{};
		VelocityData<LWP::Math::Vector3>	PVelocityTranslate{};
		EasingData<LWP::Math::Vector3>		PEasingTranslate{};
		VelocityData<LWP::Math::Vector3>	PVelocityRotate{};
		EasingData<LWP::Math::Vector3>		PEasingScale{};
		EasingData<LWP::Math::Vector4>		PEasingColor{};

		void AddValue(const std::string& id, LWP::Utility::JsonIO& jsonIO) {
			// ID名でグループ開始
			jsonIO.BeginGroup(id)
				.AddValue("ParticleType", &ParticleType)
				.AddValue("ModelPath", &ModelPath)
				.AddValue("SurfaceType", &SurfaceType)
				.AddValue("TexPath", &TexPath)
				.AddValue("SplitSize", &SplitSize)
				.AddValue("AnimTime", &AnimTime)
				.AddValue("IsAnimLoop", &IsAnimLoop)
				.AddValue("EmitAliveTime", &EmitAliveTime)
				.AddValue("IsWaitDeleteAllParticles", &IsWaitDeleteAllParticles)
				.AddValue("EmitCount", &EmitCount)
				.AddValue("MaxEmitCount", &MaxEmitCount)
				.AddValue("EmitTime", &EmitTime)
				.AddValue("unificationRandomScale", &unificationRandomScale)

				.AddValue("EmitTimeAmp-Min", &EmitTimeAmp.min)
				.AddValue("EmitTimeAmp-Max", &EmitTimeAmp.max)

				.AddValue("AliveTimeAmp-Min", &AliveTimeAmp.min)
				.AddValue("AliveTimeAmp-Max", &AliveTimeAmp.max)

				.AddValue("PVelocityT-IsUsed", &PVelocityTranslate.isUsed)
				.AddValue("PVelocityT-Start", &PVelocityTranslate.startValue)
				.AddValue("PVelocityT-StartAmp-Min", &PVelocityTranslate.startValueAmp.min)
				.AddValue("PVelocityT-StartAmp-Max", &PVelocityTranslate.startValueAmp.max)
				.AddValue("PVelocityT-Velocity", &PVelocityTranslate.velocity)
				.AddValue("PVelocityT-VelocityAmp-Min", &PVelocityTranslate.velocityAmp.min)
				.AddValue("PVelocityT-VelocityAmp-Max", &PVelocityTranslate.velocityAmp.max)
				.AddValue("PVelocityT-Acceleration", &PVelocityTranslate.acceleration)
				.AddValue("PVelocityT-AccelerationAmp-Min", &PVelocityTranslate.accelerationAmp.min)
				.AddValue("PVelocityT-AccelerationAmp-Max", &PVelocityTranslate.accelerationAmp.max)

				.AddValue("PEasingT-IsUsed", &PEasingTranslate.isUsed)
				.AddValue("PEasingT-Start", &PEasingTranslate.startValue)
				.AddValue("PEasingT-StartAmp-Min", &PEasingTranslate.startValueAmp.min)
				.AddValue("PEasingT-StartAmp-Max", &PEasingTranslate.startValueAmp.max)
				.AddValue("PEasingT-End", &PEasingTranslate.endValue)
				.AddValue("PEasingT-EndAmp-Min", &PEasingTranslate.endValueAmp.min)
				.AddValue("PEasingT-EndAmp-Max", &PEasingTranslate.endValueAmp.max)
				.AddValue("PEasingT-EasingType", &PEasingTranslate.eType)

				.AddValue("PVelocityR-IsUsed", &PVelocityRotate.isUsed)
				.AddValue("PVelocityR-Start", &PVelocityRotate.startValue)
				.AddValue("PVelocityR-StartAmp-Min", &PVelocityRotate.startValueAmp.min)
				.AddValue("PVelocityR-StartAmp-Max", &PVelocityRotate.startValueAmp.max)
				.AddValue("PVelocityR-Velocity", &PVelocityRotate.velocity)
				.AddValue("PVelocityR-VelocityAmp-Min", &PVelocityRotate.velocityAmp.min)
				.AddValue("PVelocityR-VelocityAmp-Max", &PVelocityRotate.velocityAmp.max)
				.AddValue("PVelocityR-Acceleration", &PVelocityRotate.acceleration)
				.AddValue("PVelocityR-AccelerationAmp-Min", &PVelocityRotate.accelerationAmp.min)
				.AddValue("PVelocityR-AccelerationAmp-Max", &PVelocityRotate.accelerationAmp.max)

				.AddValue("PEasingS-IsUsed", &PEasingScale.isUsed)
				.AddValue("PEasingS-Start", &PEasingScale.startValue)
				.AddValue("PEasingS-StartAmp-Min", &PEasingScale.startValueAmp.min)
				.AddValue("PEasingS-StartAmp-Max", &PEasingScale.startValueAmp.max)
				.AddValue("PEasingS-End", &PEasingScale.endValue)
				.AddValue("PEasingS-EndAmp-Min", &PEasingScale.endValueAmp.min)
				.AddValue("PEasingS-EndAmp-Max", &PEasingScale.endValueAmp.max)
				.AddValue("PEasingS-EasingType", &PEasingScale.eType)

				.AddValue("PEasingC-IsUsed", &PEasingColor.isUsed)
				.AddValue("PEasingC-Start", &PEasingColor.startValue)
				.AddValue("PEasingC-StartAmp-Min", &PEasingColor.startValueAmp.min)
				.AddValue("PEasingC-StartAmp-Max", &PEasingColor.startValueAmp.max)
				.AddValue("PEasingC-End", &PEasingColor.endValue)
				.AddValue("PEasingC-EndAmp-Min", &PEasingColor.endValueAmp.min)
				.AddValue("PEasingC-EndAmp-Max", &PEasingColor.endValueAmp.max)
				.AddValue("PEasingC-EasingType", &PEasingColor.eType);

			jsonIO.EndGroup();
		}
	};
}
