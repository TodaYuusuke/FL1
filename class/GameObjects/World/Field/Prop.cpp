#include "Prop.h"

using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Base;

Prop::Prop(const std::string& name, const std::string& filePath, const float radius, const int priority)
{
	// 名称取得
	name_ = name;

	// ファイルパス受け取り
	data_.ModelPath = filePath;

	// ファイルパスが入力されている場合
	if (data_.ModelPath != "") {
		// モデル読み込み
		model_.LoadShortPath("level/" + data_.ModelPath);
	}
	else {
		// 仮モデル読み込み
		model_.LoadShortPath("level/TempCube/TempCube.gltf");
	}

	// 各種値の受け取り
	priority_ = priority;
	radius_ = radius;

	colliderSphere_.LoadSphere();
	colliderSphere_.worldTF.Parent(&model_.worldTF);
}

Prop::Prop(const LWP::Prop::PropSaveData& data)
{
	// データ受け取り
	data_ = data;

	// 名称取得
	name_ = data_.Name;
	// モデル読み込み
	model_.LoadShortPath("level/" + data_.ModelPath);
	// 座標読み取り
	model_.worldTF.translation	= data_.Pos;
	model_.worldTF.rotation		= data_.Rotate;
	model_.worldTF.scale		= data_.Scale;
	// 優先度取得
	priority_	= data_.Priority;
	radius_		= data_.Radius;

	colliderSphere_.LoadSphere();
	colliderSphere_.worldTF.Parent(&model_.worldTF);
}

void Prop::DrawGui()
{
	// 設定開始
	ImGui::NewLine();
	ImGui::Separator();
	ImGui::Text("Prop Settings");
	ImGui::Separator();
	ImGui::NewLine();

	// 名称設定
	ImGuiManager::InputText("Prop Name", name_);
	ImGui::NewLine();

	// モデルのファイルパス設定と再読み込み
	ImGuiManager::InputText("ModelPath", data_.ModelPath);
	ImGui::SameLine();
	if (ImGui::Button("Reload")) { model_.LoadShortPath("level/" + data_.ModelPath); }
	ImGui::NewLine();

	// 座標設定
	model_.worldTF.DebugGUI();
	ImGui::NewLine();
	// 優先度設定
	ImGui::InputInt("Priority", &priority_);
	if (priority_ < 0) { priority_ = 0; }

	// 半径設定
	ImGui::DragFloat("ColliderRadius", &radius_, 0.01f);
	ImGui::NewLine();

	colliderSphere_.worldTF.scale = { radius_, radius_, radius_ };

	// ボタンを押したら削除
	ImGui::NewLine();
	if (ImGui::Button("Delete This")) { isAlive_ = false; }
}

void Prop::ImGuiRadioButton(int& id, int& buttonID, Prop*& targetProp)
{
	// ImGUI上で選択状態であるならばUIの色を変更する
	if (isSelected_) {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 255, 0, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 255, 0, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 255, 0, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0, 255, 0, 1.0f));
	}

	// ラジオボタンの表示
	std::string displayName = std::to_string(buttonID) + " : " + name_;
	if (ImGui::RadioButton(displayName.c_str(), &id, buttonID)) {
		// 表示対象として登録
		targetProp = this;
	}

	// 変更した色を元に戻す必要があるためここで戻す
	if (isSelected_) {
		ImGui::PopStyleColor(4);
	}

	// 選択フラグリセット
	isSelected_ = false;

	// ボタンID加算
	buttonID++;
}

LWP::Prop::PropSaveData* Prop::GetData()
{
	// 各種情報の入力
	data_.Name			= name_;
	data_.Pos			= model_.worldTF.translation;
	data_.Rotate		= model_.worldTF.rotation;
	data_.Scale			= model_.worldTF.scale;
	data_.Priority		= priority_;
	data_.Radius		= radius_;

	// 返還
	return &data_;
}

