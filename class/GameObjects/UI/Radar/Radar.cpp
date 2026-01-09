#include "Radar.h"

void Radar::Initialize() {
	colorSample_[PLAYER] = { 0,0,255,255 };
	colorSample_[ENEMY] = { 255,0,0,255 };
	colorSample_[WEAPON] = { 0,255,0,255 };

	//void (Radar::*p_func)(LWP::Math::Vector3);
	//p_func =Radar::AppendTargetEnemy;

	player_.LoadCube();
	player_.worldTF.Parent(&centerTransform_);
	for (auto& material : player_.materials) {
		material.second.color = colorSample_[PLAYER];
	}
	for (size_t i = 0; i < kMaxModels_; i++) {
		unitModels_[i].LoadCube();
		unitModels_[i].worldTF.Parent(&player_.worldTF);
		unitModels_[i].SetAllMaterialLighting(false);
	}

	// 管理クラスの調整項目
	json_.Init(kJsonDirectoryPath + "MiniMap.json")
		//.BeginGroup("Data")
		.AddValue<float>("Scale", &mapScale_)
		.AddValue<float>("Border", &viewBorder_)
		//.EndGroup()
		.BeginGroup("Center")
		.AddValue<LWP::Math::Vector3>("Scale", &centerTransform_.scale)
		.AddValue<LWP::Math::Quaternion>("Rotate", &centerTransform_.rotation)
		.AddValue<LWP::Math::Vector3>("Translate", &centerTransform_.translation)
		.EndGroup()

		.CheckJsonFile();
}

void Radar::Update() {
	//格納されてるデータ分モデルを有効にする
	size_t index=0;
	for (size_t i = 0; i < kMaxModels_;i++) {
		unitModels_[i].isActive = false;
	}
	for (auto &data : unitDatas_) {
		if (index >= unitDatas_.size())break;
		for (auto& material : unitModels_[index].materials) {
			material.second.color = colorSample_[unitDatas_[index].type];
		}
		LWP::Math::Matrix4x4 mat = playerTransform_->GetAffineMatrix();

		LWP::Math::Vector3 newVec = LWP::Math::Matrix4x4::TransformCoord(unitDatas_[index].position, mat.Inverse());
		newVec.y = 0;
		newVec *= mapScale_;
		if (newVec.Length() > viewBorder_) {
			index++;
			continue;
		}
		unitModels_[index].isActive = true;
		unitModels_[index].worldTF.translation = newVec;
		unitModels_[index].worldTF.Parent(&centerTransform_);
		index++;
	}
}

void Radar::DebugGui() {
	if (ImGui::BeginTabItem("MiniMap")) {
		json_.DebugGUI();
		ImGui::EndTabItem();
	}
}