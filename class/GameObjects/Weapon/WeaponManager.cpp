#include "WeaponManager.h"
#include "Gun/MachineGun/MachineGun.h"
#include "Gun/ShotGun/ShotGun.h"
#include "Gun/Rifle/Rifle.h"
#include "Gun/Launcher/Launcher.h"
#include "Gun/Missile/Missile.h"
#include "Melee/Melee.h"
#include "../World/World.h"
#include "../Player/Player.h"
#include "../Attack/AttackConfig.h"
#include "../../Componets/Input/VirtualController.h"
#include "../../Componets/InputMyController/ControllerReceiver.h"
#include <queue>

using namespace LWP;
using namespace LWP::Math;
using namespace FLMath;

WeaponManager::WeaponManager() {
	// 武器種生成プレビュー
	for (int i = 0; i < WeaponConfig::Rarity::rarity.size(); i++) {
		weaponTypePreview_.push_back(WeaponConfig::Name::name[i]);
	}
	// 武器レアリティ生成プレビュー
	for (int i = 0; i < WeaponConfig::Rarity::rarity.size(); i++) {
		weaponRarityPreview_.push_back(WeaponConfig::Rarity::rarity[i]);
	}

	// コピー元の武器作成
	CreateOriginWeapon();

	// 拾える武器画像
	for (int i = 0; i < pickUpWeaponSprite_.size(); i++) {
		pickUpWeaponSprite_[i].LoadTexture(WeaponConfig::TextureName::UI::uiName[i]);
		pickUpWeaponSprite_[i].material.color = { 1.0f,1.0f,1.0f,0.5f };
	}
	samplePickUpWeaponSprite_.isActive = false;
	samplePickUpWeaponSprite_.anchorPoint = { 0.5f, 0.5f };
	samplePickUpWeaponSprite_.worldTF.translation = { 400.0f, 200.0f,0.0f };
	samplePickUpWeaponSprite_.worldTF.scale = { 1.5f, 1.5f,1.0f };

	//ボタン表記
	for (int i = 0; i < pickUpUISprite_.size(); i++) {
		pickUpUISprite_[i].LoadTexture(controllerUIName[i]);
	}
}

WeaponManager::~WeaponManager() {
	// 現在の武器
	for (IWeapon* weapon : weapons_) {
		delete weapon;
	}
}

void WeaponManager::Init() {
	for (IWeapon* weapon : weapons_) {
		weapon->Init();
	}
}

void WeaponManager::Update() {
	for (IWeapon* weapon : weapons_) {
		weapon->Update();
		if(!weapon->GetActor()) appendMiniMap_(weapon->GetWorldTF()->GetWorldPosition());
	}

	for (int i = 0; i < pickUpWeaponSprite_.size(); i++) {
		pickUpWeaponSprite_[i].isActive = false;
		pickUpWeaponSprite_[i].worldTF.translation = samplePickUpWeaponSprite_.worldTF.translation;
		pickUpWeaponSprite_[i].worldTF.scale = samplePickUpWeaponSprite_.worldTF.scale;
		pickUpWeaponSprite_[i].anchorPoint = samplePickUpWeaponSprite_.anchorPoint;
	}

	for (int i = 0; i < pickUpUISprite_.size(); i++) {
		pickUpUISprite_[i].isActive = false;
		pickUpUISprite_[i].worldTF.translation = samplePickUpWeaponSprite_.worldTF.translation;
		pickUpUISprite_[i].worldTF.scale = samplePickUpWeaponSprite_.worldTF.scale;
		pickUpUISprite_[i].anchorPoint = samplePickUpWeaponSprite_.anchorPoint;
	}
	//入力タイプ確認
	controllerType_ = kJoyPad;
	if (ControllerReceiver::GetInstance()->IsOpen()) {
		controllerType_ = kLever;
	}

	// 自機が近いなら武器を渡す
	CheckPlayerToWeaponDistance();
}

void WeaponManager::DebugGui() {
	if (ImGui::BeginTabItem("WeaponManager")) {
		// 調整項目
		if (ImGui::TreeNode("Json")) {
			ImGui::Text("Select modify weapon");
			static int selectWeapon = 0;
			static int selectRarity = 0;
			// 武器種
			SelectWeaponType(selectWeapon, "Type");
			// レアリティ
			SelectWeaponRarity(selectRarity, "Rarity");

			// 選択した武器を調整
			SelectWeaponDataGui(jsonDatas_[(WeaponType)selectWeapon][(RarityType)selectRarity], orizinWeaponData_[(WeaponType)selectWeapon][(RarityType)selectRarity]);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("PickUpSprite")) {
			samplePickUpWeaponSprite_.DebugGUI();
			ImGui::TreePop();
		}

		// 武器作成
		if (ImGui::TreeNode("Create Weapon")) {
			ImGui::Text("Select create weapon");
			static int selectWeapon = 0;
			static int selectRarity = 0;
			// 武器種
			SelectWeaponType(selectWeapon, "Type");
			// レアリティ
			SelectWeaponRarity(selectRarity, "Rarity");

			// 生成座標
			ImGui::DragFloat3("CreatePos", &createPos_.x, 0.01f);

			// 武器の四散範囲
			ImGui::DragFloat3("WeaponDropRange", &weaponDropVel.x, 0.01f);
			ImGui::DragFloat("PickUpWeaponAngle", &pickUpWeaponAngle, 0.01f);
			ImGui::DragFloat("PickUpWeaponRange", &pickUpWeaponRange, 0.01f);

			// 武器生成
			if (ImGui::Button("Create")) {
				// コピー元の武器情報
				createWeaponData_ = orizinWeaponData_[(WeaponType)selectWeapon][(RarityType)selectRarity];
				IWeapon* weapon = CreateSelectedWeapon(selectWeapon);
				weapons_.push_back(weapon);
				// 武器を地面に出す
				DropWeapon(weapon);
			}

			// 武器解放
			if (ImGui::Button("All Delete")) {
				for (IWeapon* weapon : weapons_) {
					// 所持者がいるなら解放しない
					if (weapon->GetActor()) { continue; }
					DeleteWeapon(weapon);
				}
			}

			ImGui::TreePop();
		}

		ImGui::EndTabItem();
	}
}

void WeaponManager::CheckPlayerToWeaponDistance() {
	// 回収キーを入力していなければ終了
	//if (!VirtualController::GetInstance()->GetPress(BindActionType::kCollect)) { return; }
	// 武器が存在してないなら終了
	if (weapons_.empty()) { return; }

	// 最も近い武器を探す比較関数
	auto compare = [&](IWeapon* w, IWeapon* nextW) {
		Vector3 playerPos = pWorld_->FindActor("Player")->GetWorldTF()->GetWorldPosition();

		Vector3 dir1 = w->GetWorldTF()->GetWorldPosition() - playerPos;
		Vector3 dir2 = nextW->GetWorldTF()->GetWorldPosition() - playerPos;

		// y を無視して2D距離計算
		dir1.y = 0.0f;
		dir2.y = 0.0f;

		float dist1 = dir1.Length();
		float dist2 = dir2.Length();

		// 昇順
		return dist1 > dist2;
		};
	// 自機から近い順に武器を整列
	// カメラ内もしくは自機の方向ベクトルから一定の角度内にある武器対象
	std::priority_queue<
		IWeapon*,               // 要素の型
		std::vector<IWeapon*>,  // 内部コンテナ
		decltype(compare)       // コンパレータ
	> que{ compare };

	// 回収できる武器を判別
	for (IWeapon* weapon : weapons_) {
		// 所有者がいるなら終了
		if (weapon->GetActor()) { continue; }
		// 武器が可視化されていないなら終了
		if (!weapon->GetModel().isActive) { continue; }
		// 一定距離内にいるなら回収
		Vector2 playerPos = {
			player_->GetWorldTF()->GetWorldPosition().x,
			player_->GetWorldTF()->GetWorldPosition().z
		};
		Vector2 weaponPos = {
			weapon->GetWorldTF()->GetWorldPosition().x,
			weapon->GetWorldTF()->GetWorldPosition().z
		};
		if (Vector2::Distance(playerPos, weaponPos) > std::powf(pickUpWeaponRange, 2.0f)) { continue; }
		// 自機の方向ベクトルと自機~武器の方向ベクトルの内積を求める
		Vector3 wDir = (weapon->GetWorldTF()->GetWorldPosition() - pWorld_->FindActor("Player")->GetWorldTF()->GetWorldPosition()).Normalize();
		Vector3 pDir = (Vector3{ 0.0f,0.0f,1.0f } *Matrix4x4::CreateRotateXYZMatrix(pWorld_->FindActor("Player")->GetWorldTF()->rotation)).Normalize();
		float dot = Vector3::Dot(wDir, pDir);
		if (dot <= pickUpWeaponAngle) { continue; }

		// 回収可能
		que.push(weapon);
	}

	// ***** 回収可能武器がないなら終了 ***** //
	if (que.empty()) { return; }
	// 拾える武器表示
	pickUpWeaponSprite_[WeaponConfig::GetWeaponType(que.top()->GetName())].isActive = true;
	pickUpUISprite_[(int)controllerType_].isActive = true;
	if (!VirtualController::GetInstance()->GetPress(BindActionType::kCollect)) { return; }//お試し
	// 武器を拾う
	PickUpWeapon(que.top(), pWorld_->FindActor("Player"));
}

IWeapon* WeaponManager::CreateSelectedWeapon(int weaponType) {
	switch (weaponType) {
	case (int)WeaponType::kMachineGun:
		return CreateMachineGun();
		break;
	case (int)WeaponType::kShotGun:
		return CreateShotGun();
		break;
	case (int)WeaponType::kRifle:
		return CreateRifle();
		break;
	case (int)WeaponType::kLauncher:
		return CreateLauncher();
		break;
	case (int)WeaponType::kMissile:
		return CreateMissile();
		break;
	case (int)WeaponType::kMelee:
		return CreateMelee();
		break;
	default:
		return nullptr;
		break;
	}
}

std::string WeaponManager::ConvertWeaponModelName(int weaponType, int weaponRarity) {
	return WeaponConfig::ModelName::modelName[weaponType][weaponRarity];
}

std::string WeaponManager::ConvertWeaponTypeName(int type) {
	return WeaponConfig::Name::name[type];
}

std::string WeaponManager::ConvertWeaponRarityName(int rarity) {
	return WeaponConfig::Rarity::rarity[rarity];
}

void WeaponManager::CreateOriginWeapon() {
	for (int i = 0; i < weaponTypePreview_.size(); i++) {
		// 武器種名
		std::string weaponType = ConvertWeaponTypeName(i);
		for (int j = 0; j < weaponRarityPreview_.size(); j++) {
			// レアリティ名
			std::string rarityType = ConvertWeaponRarityName(j);
			orizinWeaponData_[(WeaponType)i][(RarityType)j].name = weaponType;
			orizinWeaponData_[(WeaponType)i][(RarityType)j].modelName = ConvertWeaponModelName(i, j);
			orizinWeaponData_[(WeaponType)i][(RarityType)j].rarity = j;

			// 調整データ作成
			CreateJsonData(jsonDatas_[(WeaponType)i][(RarityType)j], orizinWeaponData_[(WeaponType)i][(RarityType)j], weaponType + "_" + rarityType);
		}
	}
}

MachineGun* WeaponManager::CreateMachineGun() {
	MachineGun* gun = new MachineGun(createWeaponData_);
	return gun;
}

ShotGun* WeaponManager::CreateShotGun() {
	ShotGun* gun = new ShotGun(createWeaponData_);

	return gun;
}

Rifle* WeaponManager::CreateRifle() {
	Rifle* gun = new Rifle(createWeaponData_);

	return gun;
}

Launcher* WeaponManager::CreateLauncher() {
	Launcher* gun = new Launcher(createWeaponData_);

	return gun;
}

Missile* WeaponManager::CreateMissile() {
	Missile* gun = new Missile(createWeaponData_);

	return gun;
}

Melee* WeaponManager::CreateMelee() {
	Melee* melee = new Melee(createWeaponData_);

	return melee;
}

void WeaponManager::CreateJsonData(LWP::Utility::JsonIO& json, WeaponData& data, const std::string& name) {
	// ファイル名
	std::string fileName = kJsonDirectoryPath + data.name + "/" + name + ".json";
	json.Init(fileName)
		// 発射間隔
		.BeginGroup("Interval")
		.AddValue<float>("Normal", &data.shotIntervalTime)
		.AddValue<float>("Burst", &data.burstIntervalTime)
		.EndGroup()

		// 弾
		.BeginGroup("Bullet")
		// 弾数
		.AddValue<int>("Num", &data.bulletNum)
		// 弾の種類
		.AddValue<int>("Type", &data.bulletType)
		// 同時に出る弾数
		.AddValue<int>("SameBulletNum", &data.sameBulletNum)
		// 拡散範囲
		.AddValue<Vector3>("DiffusingRange", &data.diffusingBulletRange)
		.EndGroup()

		// バースト数
		.AddValue<int>("BurstNum", &data.burstNum)

		// 攻撃時の練度上昇量
		.AddValue<float>("AttackSkillGain", &data.attackSkillGain)

		// 溜め時間
		.AddValue<float>("StoreTime", &data.storeTime)
		// 撃てない時間
		.AddValue<float>("CoolTime", &data.coolTime)
		// リロード時間
		.AddValue<float>("ReloadTime", &data.reloadTime)

		// レアリティ
		.AddValue<int>("Rarity", &data.rarity)
		.CheckJsonFile();
}

void WeaponManager::SelectWeaponDataGui(LWP::Utility::JsonIO& json, WeaponData& data) {
	// 調整項目
	if (ImGui::TreeNode("Json")) {
		if (ImGui::Button("Save")) {
			json.Save();
		}
		if (ImGui::Button("Load")) {
			json.Load();
		}

		// 発射間隔
		if (ImGui::TreeNode("Interval")) {
			ImGui::DragFloat("Normal", &data.shotIntervalTime, 0.01f, 0.0f);
			ImGui::DragFloat("Burst", &data.burstIntervalTime, 0.01f, 0.0f);
			ImGui::TreePop();
		}
		// 弾
		if (ImGui::TreeNode("Bullet")) {
			// 弾数
			ImGui::DragInt("Num", &data.bulletNum, 1, 0);
			// 弾の種類
			SelectBulletType(data.bulletType, "BulletType");
			// 同時に出る弾数
			ImGui::DragInt("SameNum", &data.sameBulletNum, 1, 0);
			// 弾の拡散範囲
			ImGui::DragFloat3("DiffusingRange", &data.diffusingBulletRange.x, 0.01f, 0.0001f, 1.0f);
			ImGui::TreePop();
		}
		// バースト数
		ImGui::DragInt("BurstNum", &data.burstNum, 1, 0);
		// 攻撃時の練度上昇量
		ImGui::DragFloat("AttackSkillGain", &data.attackSkillGain);
		// 溜め時間
		ImGui::DragFloat("StoreTime", &data.storeTime);
		// 撃てない時間
		ImGui::DragFloat("CoolTime", &data.coolTime);
		// リロード時間
		ImGui::DragFloat("ReloadTime", &data.reloadTime);

		ImGui::TreePop();
	}
}

void WeaponManager::SelectWeaponType(int& selectedWeaponType, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponTypePreview_.empty()) {
		const char* combo_preview_value = weaponTypePreview_[selectedWeaponType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < weaponTypePreview_.size(); n++) {
				const bool is_selected = ((int)selectedWeaponType == n);
				std::string selectableLabel = weaponTypePreview_[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedWeaponType = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found weapon"));
	}
}

void WeaponManager::SelectWeaponRarity(int& selectedWeaponRarity, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	if (!weaponRarityPreview_.empty()) {
		const char* combo_preview_value = weaponRarityPreview_[selectedWeaponRarity].c_str();
		if (ImGui::BeginCombo((label.c_str()), combo_preview_value)) {
			for (int n = 0; n < weaponRarityPreview_.size(); n++) {
				const bool is_selected = ((int)selectedWeaponRarity == n);
				if (ImGui::Selectable(weaponRarityPreview_[n].c_str(), is_selected)) {
					selectedWeaponRarity = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::TextDisabled(("Not found behavior-tree file"));
	}
}

void WeaponManager::SelectBulletType(int& selectedType, std::string label) {
	// 読み込むbehaviorTreeのプレビュー作成
	bulletTypePreview_ = AttackManager::GetInstance()->GetBulletTypePreview();
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

void WeaponManager::DropWeapon(IWeapon* weapon) {
	if (!weapon) return;

	// 攻撃倍率を初期化
	weapon->SetAttackMultiply(1.0f);

	// 武器の所持者がいる場合
	if (weapon->GetActor()) {
		Vector3 pos = weapon->GetActor()->GetWorldTF()->GetWorldPosition() + weapon->GetWorldTF()->translation;
		// 親子付け解除
		weapon->SetParent(nullptr);
		// 座標指定
		weapon->SetTranslation(pos);
	}
	// 所持者なし
	else {
		// 座標指定
		weapon->SetTranslation(createPos_);
	}

	// 武器を四散させる
	Vector3 min = weaponDropVel * -1;
	min.y = 0.0f;
	Vector3 max = weaponDropVel;
	Vector3 randomVel = LWP::Utility::Random::GenerateVector3(min, max);
	weapon->SetVelocity(randomVel);

	// 地面に向かって武器を刺す
	Vector3 randomDir = LWP::Utility::Random::GenerateVector3(Vector3{ -0.3f,-1.0f, -0.3f }, Vector3{ 0.3f,-0.8f, 0.3f });
	Quaternion q = Quaternion::LookRotation(randomDir);
	weapon->SetRotation(q);

	// 大きくする
	weapon->SetScale(Vector3{ 1.5f,1.5f,1.5f });
}

void WeaponManager::PickUpWeapon(IWeapon* weapon, Actor* target, int weaponSide) {
	target->SetWeapon(weapon, weaponSide);
}

IWeapon* WeaponManager::CreateWeapon(int weaponType, int weaponRarity) {
	// コピー元の武器情報を設定
	createWeaponData_ = orizinWeaponData_[(WeaponType)weaponType][(RarityType)weaponRarity];

	// 選ばれた武器種作成
	IWeapon* weapon = CreateSelectedWeapon(weaponType);
	// 武器リストに追加
	weapons_.push_back(weapon);

	return weapon;
}

IWeapon* WeaponManager::CreateRandomWeapon(const std::vector<int>& weaponTypes, const std::vector<int>& weaponRarity) {
#pragma region 武器種
	int typeSize = (int)weaponTypes.size();
	int randomType = 0;
	int type = 0;
	// 1種類の場合
	if (typeSize == 1) {
		type = weaponTypes[0];
	}
	// ランダムで武器種を決める
	else if (typeSize >= 2) {
		randomType = LWP::Utility::Random::GenerateInt(0, typeSize - 1);
		type = weaponTypes[randomType];
	}
#pragma endregion

#pragma region レアリティ
	int raritySize = (int)weaponRarity.size();
	int randomRarity = 0;
	int rarity = 0;
	// 1種類の場合
	if (raritySize == 1) {
		rarity = weaponRarity[0];
	}
	// ランダムでレアリティを決める
	else if (raritySize >= 2) {
		randomRarity = LWP::Utility::Random::GenerateInt(0, raritySize - 1);
		rarity = weaponRarity[randomRarity];
	}
#pragma endregion

	// コピー元の武器情報を決定
	createWeaponData_ = orizinWeaponData_[(WeaponType)type][(RarityType)rarity];

	// 選ばれた武器種作成
	IWeapon* weapon = CreateSelectedWeapon(type);
	// 武器リストに追加
	weapons_.push_back(weapon);

	return weapon;
}

void WeaponManager::DeleteWeapon(IWeapon* weapon) {
	auto result = std::find(weapons_.begin(), weapons_.end(), weapon);
	// 存在しているなら削除
	if (result != weapons_.end()) {
		// 武器による速度
		if (weapon->GetActor()) weapon->GetActor()->SetWeaponVelocity(Vector3{ 0.0f,0.0f,0.0f });
		delete weapon;
		weapon = nullptr;
		weapons_.erase(result);
	}
}
