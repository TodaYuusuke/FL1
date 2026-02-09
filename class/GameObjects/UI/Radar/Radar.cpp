#include "Radar.h"

void Radar::Initialize() {
	colorSample_[PLAYER] = { 0,0,255,255 };
	colorSample_[ENEMY] = { 255,0,0,255 };
	colorSample_[WEAPON] = { 0,255,0,255 };
	colorSample_[PROP] = { 128,128,128,255 };

	//void (Radar::*p_func)(LWP::Math::Vector3);
	//p_func =Radar::AppendTargetEnemy;

	player_.LoadShortPath("Player/miniPlayer.gltf");
	player_.worldTF.Parent(&centerTransform_);
	for (auto& material : player_.materials) {
		material.second.color = colorSample_[PLAYER];
		material.second.enableLighting = false;
	}
	for (size_t i = 0; i < kMaxModels_; i++) {
		unitModels_[i].LoadCube();
		unitModels_[i].worldTF.Parent(&player_.worldTF);
		unitModels_[i].SetAllMaterialLighting(false);
	}
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < kDivArea_; j++) {
			outAreas_[i][j].LoadSphere();
			outAreas_[i][j].worldTF.Parent(&player_.worldTF);
			outAreas_[i][j].SetAllMaterialLighting(false);
			outAreas_[i][j].worldTF.scale = {0.5f,0.5f,0.5f};
			for (auto& material : outAreas_[i][j].materials) {
				material.second.color = {0.7f,0.1f,0.1f,1.0f};
			}
		}
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

void Radar::AddOutArea() {

	areaSides_[0][0] = { -300.0f,0.0f,-300.0f };
	areaSides_[0][1] = { -300.0f,0.0f,+300.0f };

	areaSides_[1][0] = { +300.0f,0.0f,-300.0f };
	areaSides_[1][1] = { +300.0f,0.0f,+300.0f };

	areaSides_[2][0] = { -300.0f,0.0f,-300.0f };
	areaSides_[2][1] = { +300.0f,0.0f,-300.0f };

	areaSides_[3][0] = { -300.0f,0.0f,+300.0f };
	areaSides_[3][1] = { +300.0f,0.0f,+300.0f };

	for (size_t i = 0; i < 4;i++) {
		for (size_t j = 0; j < kDivArea_;j++) {
			outAreas_[i][j].isActive = false;
		}
	}


	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < kDivArea_; j++) {
			float t = float(j) / float(kDivArea_);
			LWP::Math::Matrix4x4 mat = playerTransform_->GetAffineMatrix();

			LWP::Math::Vector3 pos = areaSides_[i][0] * (1.0f - t) + areaSides_[i][1] * t;

			//interpolation

			LWP::Math::Vector3 newVec = LWP::Math::Matrix4x4::TransformCoord(pos, mat.Inverse());
			newVec.y = -0.01f;
		
			newVec *= mapScale_;
			if (newVec.Length() > viewBorder_) {
				continue;
			}
			outAreas_[i][j].isActive = true;
			outAreas_[i][j].worldTF.translation = newVec;
			outAreas_[i][j].worldTF.Parent(&centerTransform_);
		}
	}
}

void Radar::Update() {
	//格納されてるデータ分モデルを有効にする
	size_t index=0;
	for (size_t i = 0; i < kMaxModels_;i++) {
		unitModels_[i].isActive = false;
	}
	for (auto &data : unitDatas_) {
		if (index >= unitDatas_.size() || index >= kMaxModels_)break;
		for (auto& material : unitModels_[index].materials) {
			material.second.color = colorSample_[unitDatas_[index].type];
		}
		LWP::Math::Matrix4x4 mat = playerTransform_->GetAffineMatrix();

		LWP::Math::Vector3 newVec = LWP::Math::Matrix4x4::TransformCoord(unitDatas_[index].position, mat.Inverse());
		newVec.y = 0;
		//敵を優先表示
		if (unitDatas_[index].type == ENEMY) {
			newVec.y = 0.1f;
		}
		
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

	AddOutArea();
}

void Radar::DebugGui() {
	if (ImGui::BeginTabItem("MiniMap")) {
		json_.DebugGUI();
		ImGui::EndTabItem();
	}
}