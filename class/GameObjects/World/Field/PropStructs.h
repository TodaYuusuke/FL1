#pragma once
#include <Adapter.h>

/// <summary>
/// ワールド接地物構造体
/// </summary>
namespace LWP::Prop {

	/// <summary>
	/// 接地物セーブデータ
	/// </summary>
	struct PropSaveData {
		std::string Name = "";
		std::string ModelPath = "";
		LWP::Math::Vector3 Pos{};
		LWP::Math::Quaternion Rotate{};
		LWP::Math::Vector3 Scale{};
		int Priority = 2;
		float Radius = 1.0f;
		float CapsuleRadius = 1.0f;
		float CapsuleHeight = 1.0f;

		void AddValue(const std::string& id, LWP::Utility::JsonIO& jsonIO) {
			// ID名でグループ開始
			jsonIO.BeginGroup(id)
				.AddValue("Name", &Name)
				.AddValue("ModelPath", &ModelPath)
				.AddValue("Pos", &Pos)
				.AddValue("Rotate", &Rotate)
				.AddValue("Scale", &Scale)
				.AddValue("Priority", &Priority)
				.AddValue("Radius", &Radius)
				.AddValue("CapsuleRadius", &CapsuleRadius)
				.AddValue("ColliderHeight", &CapsuleHeight);
			jsonIO.EndGroup();
		}
	};

}