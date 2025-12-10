#include "EffectEditor.h"
#include "EffectManager.h"

using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Base;
using namespace LWP::Effect;

const std::array<std::string, size_t(Emitter::ParticleType::PTypeCount)> EffectEditor::kParticleTypeMap_ = {
	"Surface",
	"model3D"
};

const std::array<std::string, size_t(Emitter::SurfaceType::STypeCount)> EffectEditor::kSurfaceTypeMap_ = {
	"Normal",
	"Billboard",
	"HorizontalBillboard",
	"VerticalBillboard",
	"StretchBillboard"
};

const std::array<std::string, size_t(Easing::Type::EasingCount)> EffectEditor::kEaseingTypeMap_ = {
	"Liner",

	"InQuint",
	"OutQuint",
	"InOutQuint",

	"InCirc",
	"OutCirc",
	"InOutCirc",

	"InCubic",
	"OutCubic",
	"InOutCubic",

	"InBack",
	"OutBack",
	"InOutBack",

	"InBounce",
	"OutBounce",
	"InOutBounce",

	"InElastic",
	"OutElastic",
	"InOutElastic",

	"InQuart",
	"OutQuart",
	"InOutQuart",

	"InExpo",
	"OutExpo",
	"InOutExpo"
};

void EffectEditor::Init()
{
	// 編集データのリセット
	editData_ = effectManager_->CreateNewData("Test Particle");
	editData_->PEasingTranslate.Convert();
	editData_->PEasingScale.Convert();
	editData_->PEasingColor.Convert();

	if (editData_->PVelocityTranslate.isUsed) {
		moveMode_ = VELOCITY;
	}
	else {
		moveMode_ = EASING;
	}
}

void EffectEditor::Update() {
	// ImGui開始
	ImGui::Begin("EffectEditor", nullptr, ImGuiWindowFlags_MenuBar);

	// メニューバー開始
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			// 新規生成
			if (ImGui::MenuItem("New")) {
				// ポップアップ表示
				popupMode_ = EffectEditor::PopUpMode::New;
			}
			// 保存
			if (ImGui::MenuItem("Save")) {
				popupMode_ = EffectEditor::PopUpMode::Save;
			}
			// 再読み込み
			if (ImGui::MenuItem("Load")) {
				LoadPMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// 現在編集中のパーティクルのパスを表示
	ImGui::Text("Now Editing Effect : ");
	ImGui::SameLine();
	ImGui::Text(nowLoadName_.c_str());
	ImGui::NewLine();

	// タブ開始
	if (ImGui::BeginTabBar("DebugTab")) {
		// 共通タブ
		if (ImGui::BeginTabItem("Common")) {
			CommonTab();
			ImGui::EndTabItem();
		}
		// 移動タブ
		if (ImGui::BeginTabItem("Move")) {
			MoveTab();
			ImGui::EndTabItem();
		}
		// 回転タブ
		if (ImGui::BeginTabItem("Rotate")) {
			RotateTab();
			ImGui::EndTabItem();
		}
		// 拡縮タブ
		if (ImGui::BeginTabItem("Scale")) {
			ScaleTab();
			ImGui::EndTabItem();
		}
		// 色タブ
		if (ImGui::BeginTabItem("Color")) {
			ColorTab();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	// デバッグ用メニュー
	ImGui::NewLine();
	ImGui::SeparatorText("DebugMenu");
	ImGui::DragFloat3("GeneratePos", &generateEmitPos_.x);

	// ボタンを押したら編集中パーティクルの生成
	if (ImGui::Button("GenerateParticle")) {
		CreateParticle();
	}

	// ポップアップ関連の更新
	PopUpUpdate();

	ImGui::End();
}

void EffectEditor::EditParticleTypeGUI(int& type)
{
	if (ImGui::BeginCombo("Particle Type Setting", kParticleTypeMap_[static_cast<uint32_t>(type)].c_str()))
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(Emitter::ParticleType::PTypeCount); ++i)
		{
			bool isSelected = (type == static_cast<Emitter::ParticleType>(i));
			if (ImGui::Selectable(kParticleTypeMap_[i].c_str(), isSelected))
			{
				type = static_cast<Emitter::ParticleType>(i);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void EffectEditor::EditSurfaceTypeGUI(int& type)
{
	if (ImGui::BeginCombo("Surface Type Setting", kSurfaceTypeMap_[static_cast<uint32_t>(type)].c_str()))
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(Emitter::SurfaceType::STypeCount); ++i)
		{
			bool isSelected = (type == static_cast<Emitter::SurfaceType>(i));
			if (ImGui::Selectable(kSurfaceTypeMap_[i].c_str(), isSelected))
			{
				type = static_cast<Emitter::SurfaceType>(i);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void EffectEditor::EditEasingTypeGUI(LWP::Utility::Easing::Type& type, int& eType)
{
	if (ImGui::BeginCombo("Easing Setting", kEaseingTypeMap_[static_cast<uint32_t>(type)].c_str()))
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(Easing::Type::EasingCount); ++i)
		{
			bool isSelected = (type == static_cast<Easing::Type>(i));
			if (ImGui::Selectable(kEaseingTypeMap_[i].c_str(), isSelected))
			{
				type = static_cast<Easing::Type>(i);
				eType = static_cast<int>(type);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void EffectEditor::EditVector3GUI(const std::string& id, LWP::Math::Vector3& data, const bool isUnification, const bool isClampZero)
{
	// 均一化を行う場合
	if (isUnification) {
		// GUI表示
		ImGui::DragFloat(id.c_str(), &data.x, 0.1f);
		// 均一化
		data = Vector3(data.x, data.x, data.x);
	}
	else {
		// GUI表示
		ImGui::DragFloat3(id.c_str(), &data.x, 0.1f);
	}

	// ゼロでクランプする場合
	if (isClampZero) {
		if (data.x < 0.0f) { data.x = 0.0f; }
		if (data.y < 0.0f) { data.y = 0.0f; }
		if (data.z < 0.0f) { data.z = 0.0f; }
	}
}

void EffectEditor::EditColorGUI(const std::string& id, LWP::Math::Vector4& color)
{
	// 調整
	ImGui::DragFloat4(id.c_str(), &color.x, 0.01f);
	ImGui::SameLine();

	// 0以下になっていた場合調整
	if (color.x < 0.0f) { color.x = 0.0f; }
	if (color.y < 0.0f) { color.y = 0.0f; }
	if (color.z < 0.0f) { color.z = 0.0f; }
	if (color.w < 0.0f) { color.w = 0.0f; }

	// 色変換し、その結果を表示
	ImVec4 imColor = { color.x, color.y, color.z, color.w };
	std::string previewId = id + " : Preview";
	ImGui::ColorButton(previewId.c_str(), imColor);
}

void EffectEditor::EditVelocityGUI(const std::string& id, LWP::Effect::VelocityData<LWP::Math::Vector3>& data, const bool isUnification)
{
	// ID名称設定
	std::string ampId = id + " Amp";

	// 開始値設定
	EditVector3GUI(id, data.startValue, isUnification);
	EditVector3GUI(ampId, data.startValueAmp.max, isUnification, true);
	data.startValueAmp.min = data.startValueAmp.max * -1.0f;
	ImGui::Separator();

	// 速度設定
	EditVector3GUI("Velocity", data.velocity, isUnification);
	EditVector3GUI("Velocity Amp", data.velocityAmp.max, isUnification, true);
	data.velocityAmp.min = data.velocityAmp.max * -1.0f;
	ImGui::Separator();

	// 加速度設定
	EditVector3GUI("Acceleration", data.acceleration, isUnification);
	EditVector3GUI("Acceleration Amp", data.accelerationAmp.max, isUnification, true);
	data.accelerationAmp.min = data.accelerationAmp.max * -1.0f;
}

void EffectEditor::EditEasingGUI(const std::string& id, LWP::Effect::EasingData<LWP::Math::Vector3>& data, const bool isUnification)
{
	// 開始値設定
	EditVector3GUI("Start", data.startValue, isUnification);
	EditVector3GUI("Start Amp", data.startValueAmp.max, isUnification, true);
	data.startValueAmp.min = data.startValueAmp.max * -1.0f;
	ImGui::Separator();

	// 終端値設定
	EditVector3GUI("End", data.endValue, isUnification);
	EditVector3GUI("End Amp", data.endValueAmp.max, isUnification, true);
	data.endValueAmp.min = data.endValueAmp.max * -1.0f;
	ImGui::Separator();

	// 補間設定
	EditEasingTypeGUI(data.type, data.eType);

	// 警告回避用
	id;
}

void EffectEditor::PopUpUpdate()
{
	// 中央に表示する（オプション）
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// ウィンドウのスケール調整
	ImGui::SetWindowFontScale(1.5f);

	switch (popupMode_)
	{
	case EffectEditor::None:
		break;
	case EffectEditor::New:
		// ポップアップ表示
		ImGui::OpenPopup("New");

		// ポップアップの起動
		if (ImGui::BeginPopupModal("New", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

			// 名前の入力を指示
			ImGui::Text("Input New Effect Name.");
			LWP::Base::ImGuiManager::InputText("New Name", createEffectName_);
			
			if (createEffectName_ != "") {
				if (ImGui::Button("Create")) {
					// 新規作成
					editData_ = effectManager_->CreateNewData(createEffectName_);
					nowLoadName_ = createEffectName_;
					editData_->PEasingTranslate.Convert();
					editData_->PEasingScale.Convert();
					editData_->PEasingColor.Convert();

					if (editData_->PVelocityTranslate.isUsed) {
						moveMode_ = VELOCITY;
					}
					else {
						moveMode_ = EASING;
					}

					// 開いたウィンドウを閉じる
					ImGui::CloseCurrentPopup();
					popupMode_ = EffectEditor::PopUpMode::None;
					createEffectName_ = "";
				}
			}
			else {
				ImGui::Text("Please Input Name.   ");
			}

			// 改行しない
			ImGui::SameLine();

			// キャンセル
			if (ImGui::Button("Cancel")) {
				// 開いたウィンドウを閉じる
				ImGui::CloseCurrentPopup();
				popupMode_ = EffectEditor::PopUpMode::None;
				createEffectName_ = "";
			}

			// ポップアップ終了
			ImGui::EndPopup();
		}
		break;
	case EffectEditor::Save:
		// ポップアップ表示
		ImGui::OpenPopup("Save");

		// ポップアップの起動
		if (ImGui::BeginPopupModal("Save", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

			// 保存するか否か確認する
			ImGui::Text("Save All Effects?");

			// はい
			if (ImGui::Button("Yes")) {
				// 保存
				effectManager_->SaveAllData();

				// 開いたウィンドウを閉じる
				ImGui::CloseCurrentPopup();
				popupMode_ = EffectEditor::PopUpMode::None;
			}

			// 改行しない
			ImGui::SameLine();

			// いいえ
			if (ImGui::Button("No")) {
				// 開いたウィンドウを閉じる
				ImGui::CloseCurrentPopup();
				popupMode_ = EffectEditor::PopUpMode::None;
			}

			// ポップアップ終了
			ImGui::EndPopup();
		}
		break;
	case EffectEditor::Load:
		break;
	}

	// ウィンドウのスケール調整
	ImGui::SetWindowFontScale(1.0f);
}

void EffectEditor::LoadPMenu()
{

}

void EffectEditor::CommonTab()
{
	ImGui::SeparatorText("Common Category");
	ImGui::NewLine();

	// 粒子タイプの編集
	EditParticleTypeGUI(editData_->ParticleType);
	// 粒子タイプによって処理を分岐
	switch (editData_->ParticleType)
	{
	case Emitter::ParticleType::Surface:
		// テクスチャパスを入力
		ImGuiManager::InputText("Tex Path", editData_->TexPath);
		ImGui::NewLine();
		// 平面タイプの選択
		EditSurfaceTypeGUI(editData_->SurfaceType);
		break;
	case Emitter::ParticleType::model3D:
		// Todo : 実装途中
		ImGui::Text("Work in Progress");
		break;
	}
	ImGui::Separator();
	
	// 生存時間の調整
	ImGui::DragFloat("Emitter Alive Time", &editData_->EmitAliveTime, 0.01f, 0.0f);
	ImGui::Checkbox("Is Wait Delete All Particles", &editData_->IsWaitDeleteAllParticles);
	ImGui::Separator();

	// 生成される粒子数
	ImGui::DragInt("Particle Emit Count", &editData_->EmitCount, 1.0f, 1);
	ImGui::DragInt("Particle Max Emit Count", &editData_->MaxEmitCount, 1.0f, 1);
	ImGui::DragFloat("Particle Emit Time", &editData_->EmitTime, 0.01f, 0.0f);
	ImGui::DragFloat("Particle Emit Time Amp", &editData_->EmitTimeAmp.max, 0.01f, 0.0f);
	editData_->EmitTimeAmp.min = -editData_->EmitTimeAmp.max;
	ImGui::Separator();

	ImGui::DragFloat("Particle Alive Time Min", &editData_->AliveTimeAmp.min, 0.01f, 0.0f);
	ImGui::DragFloat("Particle Alive Time Max", &editData_->AliveTimeAmp.max, 0.01f, 0.0f);
}

void EffectEditor::MoveTab()
{
	ImGui::SeparatorText("Move Category");
	ImGui::NewLine();

	// 文字列表示の切り替え
	std::string displayName = "";
	switch (moveMode_)
	{
	case EffectEditor::VELOCITY:
		displayName = "Velocity";
		break;
	case EffectEditor::EASING:
		displayName = "Easing";
		break;
	}
	// ラジオボタンで移動方法を切り替え
	if (ImGui::BeginMenu(displayName.c_str())) {
		ImGui::RadioButton("Velocity", &moveMode_, VELOCITY);
		ImGui::RadioButton("Easing", &moveMode_, EASING);
		ImGui::EndMenu();
	}
	ImGui::NewLine();

	// 移動モードによって切り替え
	switch (moveMode_)
	{
	case EffectEditor::VELOCITY:
		// 速度移動調整GUI
		EditVelocityGUI("Default", editData_->PVelocityTranslate);

		// 使用状態の切り替え
		editData_->PVelocityTranslate.isUsed = true;
		editData_->PEasingTranslate.isUsed = false;
		break;
	case EffectEditor::EASING:

		// 移動補間調整GUI
		EditEasingGUI("", editData_->PEasingTranslate, false);

		// 使用状態の切り替え
		editData_->PVelocityTranslate.isUsed = false;
		editData_->PEasingTranslate.isUsed = true;
		break;
	}
}

void EffectEditor::RotateTab()
{
	ImGui::SeparatorText("Rotate Category  (All Degree)");
	ImGui::NewLine();

	// 編集GUI
	EditVelocityGUI("Degree", editData_->PVelocityRotate, false);
}

void EffectEditor::ScaleTab()
{
	ImGui::SeparatorText("Scale Category");
	ImGui::NewLine();

	ImGui::Checkbox("Is Unification Scale", &editData_->unificationRandomScale);

	// 移動補間調整GUI
	EditEasingGUI("", editData_->PEasingScale, editData_->unificationRandomScale);

}

void EffectEditor::ColorTab()
{
	ImGui::SeparatorText("Color Category");
	ImGui::NewLine();

	EditColorGUI("Start Min", editData_->PEasingColor.startValueAmp.min);
	EditColorGUI("Start Max", editData_->PEasingColor.startValueAmp.max);
	ImGui::Separator();

	EditColorGUI("End Min", editData_->PEasingColor.endValueAmp.min);
	EditColorGUI("End Max", editData_->PEasingColor.endValueAmp.max);
	ImGui::Separator();

	// イージングの種類の選択
	EditEasingTypeGUI(editData_->PEasingColor.type, editData_->PEasingColor.eType);
}

void EffectEditor::CreateParticle()
{
	// マネージャーに現在の編集名称の粒子を生成するよう指示
	effectManager_->CreateNewEmitter(nowLoadName_, generateEmitPos_);
}
