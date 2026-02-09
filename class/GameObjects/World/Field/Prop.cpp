#include "Prop.h"
#include "../../Collision/CollisionMask.h"
#include "../../../Audio/AudioPlayer.h"
#include "../../../Effect/EffectManager.h"

using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Base;

Prop::Prop(const std::string& name, const std::string& filePath, const float radius, const int priority)
	 : bodyCapsule_(bodyCollision_.SetBroadShape<LWP::Object::Collider::Capsule>())
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

	// HP
	hp_ = std::make_unique<Health>(99999999999);

	// 判定生成
	Vector3 dirVec = Vector3(0.0f, 1.0f, 0.0f) * Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
	bodyCapsule_.localOffset = dirVec.Normalize() * data_.CapsuleHeight;
	bodyCapsule_.radius = data_.CapsuleRadius;
	bodyCollision_.SetFollow(&model_.worldTF);
	bodyCollision_.isActive = true;
	// 自機の所属しているマスクを設定
	bodyCollision_.mask.SetBelongFrag(GameMask::prop);
	// 当たり判定をとる対象のマスクを設定
	bodyCollision_.mask.SetHitFrag(GameMask::attack);
	bodyCollision_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		OnCollision(hitTarget);
		};

#ifdef _DEBUG

	// デバッグ時に当たり判定のコライダー表示
	colliderSphere_.isActive = true;
	colliderSphere_.worldTF.Parent(&model_.worldTF);
	colliderSphere_.ChangeFillMode();
	colliderSphere_.worldTF.scale = { radius_, radius_, radius_ };

#endif // _DEBUG

}

Prop::Prop(const LWP::Prop::PropSaveData& data)
	: bodyCapsule_(bodyCollision_.SetBroadShape<LWP::Object::Collider::Capsule>())
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
	priority_		= data_.Priority;
	radius_			= data_.Radius;
	capsuleRadius_	= data_.CapsuleRadius;
	capsuleHeight_	= data_.CapsuleHeight;

	// HP
	hp_ = std::make_unique<Health>(99999999999);

	// 判定生成
	Vector3 dirVec = Vector3(0.0f, 1.0f, 0.0f) * Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
	bodyCapsule_.localOffset = dirVec.Normalize() * data_.CapsuleHeight;
	bodyCapsule_.radius = data_.CapsuleRadius;
	bodyCollision_.SetFollow(&model_.worldTF);

	// カプセルの高さによってコライダーのONOFFを決める
	if (capsuleRadius_ <= 0.0f || capsuleHeight_ <= 0.0f) {
		bodyCollision_.isActive = false;
	}
	else {
		bodyCollision_.isActive = true;
	}
	// 自機の所属しているマスクを設定
	bodyCollision_.mask.SetBelongFrag(GameMask::prop);
	// 当たり判定をとる対象のマスクを設定
	bodyCollision_.mask.SetHitFrag(GameMask::attack);
	bodyCollision_.stayLambda = [this](LWP::Object::Collision* hitTarget) {
		OnCollision(hitTarget);
	};

	// デフォルト非表示
	colliderSphere_.LoadSphere();
	colliderSphere_.isActive = false;

#ifdef _DEBUG

	colliderSphere_.worldTF.Parent(&model_.worldTF);
	colliderSphere_.ChangeFillMode();
	colliderSphere_.worldTF.scale = { radius_, radius_, radius_ };

#endif // _DEBUG
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

	// 半径設定
	ImGui::DragFloat("BulletColliderHeight", &capsuleHeight_, 0.01f);
	ImGui::DragFloat("BulletColliderRadius", &capsuleRadius_, 0.01f);
	ImGui::NewLine();

	// スケールを合わせる
	colliderSphere_.worldTF.scale = { radius_, radius_, radius_ };

	// 高さを元にコライダー調整
	Vector3 dirVec = Vector3(0.0f, 1.0f, 0.0f) * Matrix4x4::CreateRotateXYZMatrix(model_.worldTF.rotation);
	bodyCapsule_.localOffset = dirVec.Normalize() * capsuleHeight_;
	bodyCapsule_.radius = capsuleRadius_;

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
	data_.Name				= name_;
	data_.Pos				= model_.worldTF.translation;
	data_.Rotate			= model_.worldTF.rotation;
	data_.Scale				= model_.worldTF.scale;
	data_.Priority			= priority_;
	data_.Radius			= radius_;
	data_.CapsuleRadius		= capsuleRadius_;
	data_.CapsuleHeight		= capsuleHeight_;

	// 返還
	return &data_;
}

void Prop::OnCollision(LWP::Object::Collision* hitTarget)
{
	//if (hitTarget->mask.GetHitFrag() != bodyCollision_.mask.GetBelongFrag()) { return; }
	hp_->SetIsHit(true);
	// 多重被弾回避
	std::vector<std::string> name = hp_->GetDamageAttackerName();
	if (!name.empty()) {
		auto result = std::find(name.begin(), name.end(), hitTarget->name);
		if (result != name.end()) {
			return;
		}
	}

	// ダメージを受ける
	hp_->Damage(0.0f, hitTarget->name);

	// 被弾音を鳴らす
	AudioPlayer::GetInstance()->PlayRandomAudio("PropHitSound.mp3", 3, 1.0f, LWP::AudioConfig::Enviroment, hitTarget->GetWorldPosition());
	// 被弾エフェクト
	EffectManager::GetInstance()->CreateNewEmitter("Spark", hitTarget->GetWorldPosition());
	EffectManager::GetInstance()->CreateNewEmitter("RockParticle", hitTarget->GetWorldPosition());
}

