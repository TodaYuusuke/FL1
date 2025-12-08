#include "EffectEditor.h"
#include "EffectManager.h"

using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Base;

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

EffectEditor::EffectEditor(EffectManager* manager)
{
	// マネージャー取得
	effectManager_ = manager;
}

void EffectEditor::Init()
{
	// 編集データのリセット
	editData_ = EffectSaveData();
	editData_.AliveTimeAmp.min = 1.0f;
	editData_.AliveTimeAmp.max = 1.0f;
	editData_.PEasingScale.startValue = { 1.0f, 1.0f, 1.0f };
	editData_.PEasingScale.endValue = { 1.0f, 1.0f, 1.0f };
	editData_.PEasingColor.startValue = { 1.0f, 1.0f, 1.0f, 1.0f };
	editData_.PEasingColor.startValue = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void EffectEditor::Update()
{
	// ImGui開始
	ImGui::Begin("EffectManager", nullptr, ImGuiWindowFlags_MenuBar);

	// メニューバー開始
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			// 新規生成
			if (ImGui::MenuItem("New")) {
				Init();
			}
			// 保存
			if (ImGui::MenuItem("Save")) {
				SaveMenu();
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
	ImGui::Text("Now Loading Path : ");
	ImGui::Text(nowLoadFileName_.c_str());
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

	ImGui::End();
}

void EffectEditor::EditParticleTypeUI(int& type)
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

void EffectEditor::EditSurfaceTypeUI(int& type)
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

void EffectEditor::EditEasingUI(LWP::Utility::Easing::Type& type)
{
	if (ImGui::BeginCombo("Easing Setting", kEaseingTypeMap_[static_cast<uint32_t>(type)].c_str()))
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(Easing::Type::EasingCount); ++i)
		{
			bool isSelected = (type == static_cast<Easing::Type>(i));
			if (ImGui::Selectable(kEaseingTypeMap_[i].c_str(), isSelected))
			{
				type = static_cast<Easing::Type>(i);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void EffectEditor::SaveMenu()
{

}

void EffectEditor::LoadPMenu()
{

}

void EffectEditor::CommonTab()
{
	ImGui::SeparatorText("Common Category");
	ImGui::NewLine();

	// 粒子タイプの編集
	EditParticleTypeUI(editData_.ParticleType);
	// 粒子タイプによって処理を分岐
	switch (editData_.ParticleType)
	{
	case Emitter::ParticleType::Surface:
		// テクスチャパスを入力
		ImGuiManager::InputText("Tex Path", editData_.TexPath);
		ImGui::NewLine();
		// 平面タイプの選択
		EditSurfaceTypeUI(editData_.SurfaceType);
		break;
	case Emitter::ParticleType::model3D:
		// Todo : 実装途中
		ImGui::Text("Work in Progress");
		break;
	}
	ImGui::Separator();
	
	// 生存時間の調整
	ImGui::DragFloat("Emitter Alive Time", &editData_.EmitAliveTime, 0.01f, 0.0f);
	ImGui::Checkbox("Is Wait Delete All Particles", &editData_.IsWaitDeleteAllParticles);
	ImGui::Separator();

	// 生成される粒子数
	ImGui::DragInt("Particle Emit Count", &editData_.EmitCount, 1.0f, 1);
	ImGui::DragInt("Particle Max Emit Count", &editData_.MaxEmitCount, 1.0f, 1);
	ImGui::DragFloat("Particle Emit Time", &editData_.EmitTime, 0.01f, 0.0f);
	ImGui::DragFloat("Particle Emit Time Amp", &editData_.EmitTimeAmp.max, 0.01f, 0.0f);
	editData_.EmitTimeAmp.min = -editData_.EmitTimeAmp.max;
	ImGui::Separator();

	ImGui::DragFloat("Particle Alive Time Min", &editData_.AliveTimeAmp.min, 0.01f, 0.0f);
	ImGui::DragFloat("Particle Alive Time Max", &editData_.AliveTimeAmp.max, 0.01f, 0.0f);
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
		ImGui::DragFloat3("Default", &editData_.PVelocityTranslate.startValue.x, 0.1f);
		ImGui::DragFloat3("Default Amp Min", &editData_.PVelocityTranslate.startValueAmp.min.x, 0.1f);
		ImGui::DragFloat3("Default Amp Max", &editData_.PVelocityTranslate.startValueAmp.max.x, 0.1f);
		ImGui::Separator();

		ImGui::DragFloat3("Velocity", &editData_.PVelocityTranslate.velocity.x, 0.01f);
		ImGui::DragFloat3("Velocity Amp Min", &editData_.PVelocityTranslate.velocityAmp.min.x, 0.01f);
		ImGui::DragFloat3("Velocity Amp Max", &editData_.PVelocityTranslate.velocityAmp.max.x, 0.01f);
		ImGui::Separator();

		ImGui::DragFloat3("Acceleration", &editData_.PVelocityTranslate.acceleration.x, 0.01f);
		ImGui::DragFloat3("Acceleration Amp Min", &editData_.PVelocityTranslate.accelerationAmp.min.x, 0.01f);
		ImGui::DragFloat3("Acceleration Amp Max", &editData_.PVelocityTranslate.accelerationAmp.max.x, 0.01f);
		ImGui::Separator();

		// 使用状態の切り替え
		editData_.PVelocityTranslate.isUsed = true;
		editData_.PEasingTranslate.isUsed = false;
		break;
	case EffectEditor::EASING:

		ImGui::DragFloat3("Start", &editData_.PEasingTranslate.startValue.x);
		ImGui::DragFloat3("Start Min", &editData_.PEasingTranslate.startValueAmp.min.x, 0.01f);
		ImGui::DragFloat3("Start Max", &editData_.PEasingTranslate.startValueAmp.max.x, 0.01f);
		ImGui::Separator();

		ImGui::DragFloat3("End", &editData_.PEasingTranslate.endValue.x);
		ImGui::DragFloat3("End Amp Min", &editData_.PEasingTranslate.endValueAmp.min.x, 0.01f);
		ImGui::DragFloat3("End Amp Max", &editData_.PEasingTranslate.endValueAmp.max.x, 0.01f);
		ImGui::Separator();

		// イージングの種類の選択
		EditEasingUI(editData_.PEasingTranslate.type);

		// 使用状態の切り替え
		editData_.PVelocityTranslate.isUsed = false;
		editData_.PEasingTranslate.isUsed = true;
		break;
	}
}

void EffectEditor::RotateTab()
{
	ImGui::SeparatorText("Rotate Category  (All Degree)");
	ImGui::NewLine();

	ImGui::DragFloat3("Default", &editData_.PVelocityRotate.startValue.x, 0.1f);
	ImGui::DragFloat3("Default Amp Min", &editData_.PVelocityRotate.startValueAmp.min.x, 0.1f);
	ImGui::DragFloat3("Default Amp Max", &editData_.PVelocityRotate.startValueAmp.max.x, 0.1f);
	ImGui::Separator();

	ImGui::DragFloat3("Velocity", &editData_.PVelocityRotate.velocity.x, 0.01f);
	ImGui::DragFloat3("Velocity Amp Min", &editData_.PVelocityRotate.velocityAmp.min.x, 0.01f);
	ImGui::DragFloat3("Velocity Amp Max", &editData_.PVelocityRotate.velocityAmp.max.x, 0.01f);
	ImGui::Separator();

	ImGui::DragFloat3("Acceleration", &editData_.PVelocityRotate.acceleration.x, 0.01f);
	ImGui::DragFloat3("Acceleration Amp Min", &editData_.PVelocityRotate.accelerationAmp.min.x, 0.01f);
	ImGui::DragFloat3("Acceleration Amp Max", &editData_.PVelocityRotate.accelerationAmp.max.x, 0.01f);
	ImGui::Separator();
}

void EffectEditor::ScaleTab()
{
	ImGui::SeparatorText("Scale Category");
	ImGui::NewLine();

	ImGui::Checkbox("Is Unification Scale", &editData_.unificationRandomScale);

	if (editData_.unificationRandomScale) {
		ImGui::DragFloat("Start", &editData_.PEasingScale.startValue.x);
		ImGui::DragFloat("Start Min", &editData_.PEasingScale.startValueAmp.min.x, 0.01f);
		ImGui::DragFloat("Start Max", &editData_.PEasingScale.startValueAmp.max.x, 0.01f);
		ImGui::Separator();

		ImGui::DragFloat("End", &editData_.PEasingScale.endValue.x);
		ImGui::DragFloat("End Amp Min", &editData_.PEasingScale.endValueAmp.min.x, 0.01f);
		ImGui::DragFloat("End Amp Max", &editData_.PEasingScale.endValueAmp.max.x, 0.01f);
		ImGui::Separator();

		// それぞれの値を統一する
		editData_.PEasingScale.startValue		= { editData_.PEasingScale.startValue.x, editData_.PEasingScale.startValue.x , editData_.PEasingScale.startValue.x };
		editData_.PEasingScale.startValueAmp.min = { editData_.PEasingScale.startValueAmp.min.x, editData_.PEasingScale.startValueAmp.min.x , editData_.PEasingScale.startValueAmp.min.x };
		editData_.PEasingScale.startValueAmp.max = { editData_.PEasingScale.startValueAmp.max.x, editData_.PEasingScale.startValueAmp.max.x , editData_.PEasingScale.startValueAmp.max.x };
		editData_.PEasingScale.endValue		= { editData_.PEasingScale.endValue.x, editData_.PEasingScale.endValue.x , editData_.PEasingScale.endValue.x };
		editData_.PEasingScale.endValueAmp.min = { editData_.PEasingScale.endValueAmp.min.x, editData_.PEasingScale.endValueAmp.min.x , editData_.PEasingScale.endValueAmp.min.x };
		editData_.PEasingScale.endValueAmp.max = { editData_.PEasingScale.endValueAmp.max.x, editData_.PEasingScale.endValueAmp.max.x , editData_.PEasingScale.endValueAmp.max.x };
	}
	else {
		ImGui::DragFloat3("Start", &editData_.PEasingScale.startValue.x);
		ImGui::DragFloat3("Start Min", &editData_.PEasingScale.startValueAmp.min.x, 0.01f);
		ImGui::DragFloat3("Start Max", &editData_.PEasingScale.startValueAmp.max.x, 0.01f);
		ImGui::Separator();

		ImGui::DragFloat3("End", &editData_.PEasingScale.endValue.x);
		ImGui::DragFloat3("End Amp Min", &editData_.PEasingScale.endValueAmp.min.x, 0.01f);
		ImGui::DragFloat3("End Amp Max", &editData_.PEasingScale.endValueAmp.max.x, 0.01f);
		ImGui::Separator();
	}

	// イージングの種類の選択
	EditEasingUI(editData_.PEasingTranslate.type);

}

void EffectEditor::ColorTab()
{
	ImGui::SeparatorText("Scale Category");
	ImGui::NewLine();

	ImGui::DragFloat4("Start", &editData_.PEasingColor.startValue.x);
	ImGui::DragFloat4("Start Min", &editData_.PEasingColor.startValueAmp.min.x, 0.01f);
	ImGui::DragFloat4("Start Max", &editData_.PEasingColor.startValueAmp.max.x, 0.01f);
	ImGui::Separator();

	ImGui::DragFloat4("End", &editData_.PEasingColor.endValue.x);
	ImGui::DragFloat4("End Amp Min", &editData_.PEasingColor.endValueAmp.min.x, 0.01f);
	ImGui::DragFloat4("End Amp Max", &editData_.PEasingColor.endValueAmp.max.x, 0.01f);
	ImGui::Separator();
}

void EffectEditor::CreateParticle()
{
	// 新規エミッタ生成
	Emitter* newEmitter = new Emitter(LWP::Resource::LoadTexture("Particle/" + editData_.TexPath), editData_.SurfaceType, generateEmitPos_);
	// 編集値を元にエミッタの初期化
	newEmitter->Init(editData_.EmitAliveTime, editData_.EmitTime, editData_.EmitCount, editData_.MaxEmitCount)
		.SetIsWaitDeleteAllParticles(editData_.IsWaitDeleteAllParticles)
		.SetParticleAliveTimeAmp(editData_.AliveTimeAmp.min, editData_.AliveTimeAmp.max)
		.SetRotateVelocity(editData_.PVelocityRotate)
		.SetScaleEasing(editData_.PEasingScale)
		.SetColorEasing(editData_.PEasingColor);

	// 位置座標のみは別で設定
	switch (moveMode_)
	{
	case EffectEditor::VELOCITY:
		newEmitter->SetTranslateVelocity(editData_.PVelocityTranslate);
		break;
	case EffectEditor::EASING:
		newEmitter->SetTranslateEasing(editData_.PEasingTranslate);
		break;
	}

	// 生成したエミッタを渡す
	effectManager_->SendNewEmitter(newEmitter);
}
