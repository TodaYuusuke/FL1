#include "BulletManager.h"
#include "Bullet/MissleBase.h"
#include "Bullet/Bullet.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

BulletManager::BulletManager() {
	BulletRegister<Bullet>((int)BulletType::LargeCaliber);
	BulletRegister<Bullet>((int)BulletType::SmallCaliber);
	BulletRegister<MissleBase>((int)BulletType::Launcher);
	MeleeRegister<MeleeAttack>((int)BulletType::M_Pile);

	// jsonファイル作成
	for (int i = 0; i < (int)BulletType::kCount; i++) {
		bulletTypePreview_.push_back(BulletConfig::Name::name[i]);
		CreateJsonData(jsonDatas_[i], sampleData_[i], BulletConfig::Name::name[i]);
	}
}

BulletManager::~BulletManager() {
	for (auto it = bullets_.begin(); it != bullets_.end(); it++) {
		delete (*it);
	}
	bullets_.clear();
}

void BulletManager::Init() {
	for (auto it = bullets_.begin(); it != bullets_.end(); it++) {
		(*it)->Init();
	}
}

void BulletManager::Update() {
	for (auto it = bullets_.begin(); it != bullets_.end(); it++) {
		(*it)->Update();
	}
	// 削除
	bullets_.remove_if([](BulletBase* bullet) {
		if (!bullet->GetIsAlive()) {
			delete bullet;
			return true;
		}
		return false;
		});
}

void BulletManager::DebugGui() {
	if (ImGui::BeginTabItem("BulletManager")) {
		// 調整
		if (ImGui::TreeNode("Modify")) {
			static int type = 0;
			// 調整する弾を選択
			SelectBulletType(type, "Type");
			// 調整項目表示
			SelectBulletDataGui(jsonDatas_[type], sampleData_[type]);
			ImGui::TreePop();
		}

		ImGui::EndTabItem();
	}
}

void BulletManager::CreateJsonData(LWP::Utility::JsonIO& json, BulletData& data, const std::string& name) {
	// ファイル名
	std::string fileName = kJsonDirectoryPath + name + ".json";
	json.Init(fileName)
		// 弾の大きさ(当たり判定)
		.AddValue<Vector3>("ColliderSize", &data.bulletSize)
		// 攻撃力
		.AddValue<float>("AttackPower", &data.attackValue)
		// 速度
		.AddValue<float>("Speed", &data.speed)
		// 生存時間
		.AddValue<float>("ElapsedTime", &data.elapsedTime)
		.CheckJsonFile();
}

void BulletManager::SelectBulletDataGui(LWP::Utility::JsonIO& json, BulletData& data) {
	// 調整項目
	if (ImGui::TreeNode("Json")) {
		if (ImGui::Button("Save")) {
			json.Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			json.Load();
		}

		// 弾の大きさ(当たり判定)
		ImGui::DragFloat3("ColliderSize", &data.bulletSize.x, 0.01f, 0.0001f, 1.0f);
		// 弾速
		ImGui::DragFloat("Speed", &data.speed);
		// 攻撃力
		ImGui::DragFloat("AttackPower", &data.attackValue);
		// 弾の生存時間
		ImGui::DragFloat("ElapsedTime", &data.elapsedTime);
		ImGui::TreePop();
	}
}

void BulletManager::SelectBulletType(int& selectedType, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!bulletTypePreview_.empty()) {
		const char* combo_preview_value = bulletTypePreview_[selectedType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < bulletTypePreview_.size(); n++) {
				const bool is_selected = ((int)selectedType == n);
				std::string selectableLabel = bulletTypePreview_[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedType = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found bullet"));
	}
}

void BulletManager::AddBullet(BulletBase* bullet, float attackMultiply) {
	// 名前
	bullet->SetName("Bullet" + std::to_string(createID_));
	bullet->SetAttackMultiply(attackMultiply);
	bullets_.push_back(bullet);
	createID_++;
}