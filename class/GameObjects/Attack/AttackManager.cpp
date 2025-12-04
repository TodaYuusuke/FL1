#include "AttackManager.h"
#include "Bullet/Bullet.h"
#include "Explosion/Explosion.h"

using namespace FLMath;
using namespace LWP;
using namespace LWP::Math;

AttackManager::AttackManager() {
	// 弾登録
	BulletRegister<Bullet>((int)BulletType::LargeCaliber);		// 大口径
	BulletRegister<Bullet>((int)BulletType::SmallCaliber);		// 小口径
	BulletRegister<Bullet>((int)BulletType::BuckShot);			// 散弾
	BulletRegister<Bullet>((int)BulletType::Launcher);			// ランチャー弾

	// 近接攻撃登録
	MeleeRegister<MeleeAttack>((int)BulletType::M_Pile);		// パイル攻撃
	
	// 着弾時の攻撃
	ImpactRegister<Explosion>((int)ImpactType::kExplosion);

	// jsonファイル作成
	// 攻撃
	for (int i = 0; i < (int)BulletType::kCount; i++) {
		bulletTypePreview_.push_back(AttackConfig::Bullet::Name::bulletName[i]);
		CreateJsonData(jsonBulletDatas_[i], sampleBulletDatas_[i], AttackConfig::Bullet::Name::bulletName[i]);
	}
	// 着弾時の攻撃処理
	for (int i = 0; i < (int)ImpactType::kCount; i++) {
		impactTypePreview_.push_back(AttackConfig::Impact::Name::impactName[i]);
		sampleImpactDatas_[i].type = i;
		CreateJsonData(jsonImpactDatas_[i], sampleImpactDatas_[i], AttackConfig::Impact::Name::impactName[i]);
	}
	// 移動方式
	for (int i = 0; i < (int)MovementType::kCount; i++) {
		movementTypePreview_.push_back(AttackConfig::Bullet::Movement::movementName[i]);
	}
	// 属性
	for (int i = 0; i < (int)ElementType::kCount; i++) {
		bulletElementPreview_.push_back(AttackConfig::Element::elementName[i]);
		//CreateJsonData(elementJsonDatas_[i], sampleElementData_[i], AttackConfig::Element::elementName[i]);
	}
}

AttackManager::~AttackManager() {
	for (auto it = attackList_.begin(); it != attackList_.end(); it++) {
		delete (*it);
	}
	attackList_.clear();
}

void AttackManager::Init() {
	for (auto it = attackList_.begin(); it != attackList_.end(); it++) {
		(*it)->Init();
	}
}

void AttackManager::Update() {
	for (auto it = attackList_.begin(); it != attackList_.end(); it++) {
		(*it)->Update();
	}
}

void AttackManager::EndFrame() {
	// 削除
	attackList_.remove_if([](AttackBase* bullet) {
		if (!bullet->GetIsAlive()) {
			delete bullet;
			return true;
		}
		return false;
		});
}

void AttackManager::DebugGui() {
	if (ImGui::BeginTabItem("AttackManager")) {
		// 調整
		if (ImGui::TreeNode("Attack")) {
			if (ImGui::TreeNode("Bullet")) {
				static int type = 0;
				// 調整する弾を選択
				SelectBulletType(type, "BulletType");
				// 調整項目表示
				SelectBulletDataGui(jsonBulletDatas_[type], sampleBulletDatas_[type]);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Impact")) {
				static int type = 0;
				// 選択
				SelectType(impactTypePreview_, type, "ImpactType");
				// 調整項目表示
				SelectBulletDataGui(jsonImpactDatas_[type], sampleImpactDatas_[type]);
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Element")) {
			static int type = 0;
			// 調整する弾を選択
			SelectBulletElement(type, "ElementType");
			// 調整項目表示
			//SelectBulletDataGui(elementJsonDatas_[type], sampleElementData_[type]);
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}
}

void AttackManager::CreateJsonData(LWP::Utility::JsonIO& json, AttackData& data, const std::string& name) {
	// ファイル名
	std::string fileName = kJsonDirectoryPath + name + ".json";
	json.Init(fileName)
		// 弾の大きさ(当たり判定)
		.AddValue<Vector3>("ColliderSize", &data.attackSize)
		// 攻撃力
		.AddValue<float>("AttackPower", &data.attackValue)
		// 速度
		.AddValue<float>("Speed", &data.speed)
		// 生存時間
		.AddValue<float>("ElapsedTime", &data.elapsedTime)
		// 着弾時の処理
		.AddValue<int>("ImpactType", &data.impactType)
		// 移動方式
		.AddValue<int>("MovementType", &data.movementType)
		// 追尾
		.BeginGroup("Homing")
		.AddValue<float>("DerayTime", &data.derayHomingTime)
		.AddValue<float>("Accuracy", &data.homingAccuracy)
		.EndGroup()

		.CheckJsonFile();
}

void AttackManager::CreateJsonData(LWP::Utility::JsonIO& json, ImpactData& data, const std::string& name) {
	// ファイル名
	std::string fileName = kJsonDirectoryPath + name + ".json";
	json.Init(fileName)
		// 弾の大きさ(当たり判定)
		.AddValue<float>("ColliderSize", &data.attackSize)
		// 攻撃力
		.AddValue<float>("AttackPower", &data.attackValue)
		// 速度
		.AddValue<float>("Speed", &data.speed)
		// 生存時間
		.AddValue<float>("ElapsedTime", &data.elapsedTime)
		.CheckJsonFile();
}

void AttackManager::SelectBulletDataGui(LWP::Utility::JsonIO& json, AttackData& data) {
	// 調整項目
	if (ImGui::TreeNode("Json")) {
		if (ImGui::Button("Save")) {
			json.Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			json.Load();
		}

		// 着弾時の処理選択
		SelectType(impactTypePreview_, data.impactType, "##ImpactType");
		// 移動方式選択
		SelectType(movementTypePreview_, data.movementType, "##MovementType");
		// 弾の大きさ(当たり判定)
		ImGui::DragFloat3("ColliderSize", &data.attackSize.x, 0.01f, 0.0001f, 1.0f);
		// 弾速
		ImGui::DragFloat("Speed", &data.speed);
		// 攻撃力
		ImGui::DragFloat("AttackPower", &data.attackValue);
		// 弾の生存時間
		ImGui::DragFloat("ElapsedTime", &data.elapsedTime);
		if (ImGui::TreeNode("Homing")) {
			ImGui::DragFloat("DerayTime", &data.derayHomingTime);
			ImGui::DragFloat("Accuracy", &data.homingAccuracy);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void AttackManager::SelectBulletDataGui(LWP::Utility::JsonIO& json, ImpactData& data) {
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
		ImGui::DragFloat("ColliderSize", &data.attackSize, 0.01f, 0.0001f, 1.0f);
		// 弾速
		ImGui::DragFloat("Speed", &data.speed);
		// 攻撃力
		ImGui::DragFloat("AttackPower", &data.attackValue);
		// 弾の生存時間
		ImGui::DragFloat("ElapsedTime", &data.elapsedTime);
		ImGui::TreePop();
	}
}

void AttackManager::SelectBulletType(int& selectedType, std::string label) {
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

void AttackManager::SelectType(std::vector<std::string> list, int& selectedType, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!list.empty()) {
		const char* combo_preview_value = list[selectedType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < list.size(); n++) {
				const bool is_selected = ((int)selectedType == n);
				std::string selectableLabel = list[n];
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
		ImGui::TextDisabled(("Not found element"));
	}
}

void AttackManager::SelectBulletElement(int& selectedType, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!bulletElementPreview_.empty()) {
		const char* combo_preview_value = bulletElementPreview_[selectedType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < bulletElementPreview_.size(); n++) {
				const bool is_selected = ((int)selectedType == n);
				std::string selectableLabel = bulletElementPreview_[n];
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
		ImGui::TextDisabled(("Not found element"));
	}
}

void AttackManager::AddBullet(AttackBase* attack, float attackMultiply) {
	// 名前
	attack->SetName("Bullet" + std::to_string(createID_));
	attack->SetAttackMultiply(attackMultiply);
	attackList_.push_back(attack);
	createID_++;
}