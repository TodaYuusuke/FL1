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

#include "../../../DirectXGame/Externals/nameof/nameof.hpp"
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
	bulletTypePreview_ = AttackManager::GetInstance()->GetBulletTypePreview();

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
	pickUpWeaponLines_.clear();
	// 自機が近いなら武器を渡す
	CheckPlayerToWeaponDistance();

	for (int i = 0; i < pickUpWeaponLines_.size(); i++) {
		//pickUpWeaponLines_[i].worldTF.rotation = FLMath::LookRotationZLock((pWorld_->FindActor("Player")->GetModel().GetJointWorldPosition("LockOnAnchor") - weapon->GetWorldTF()->GetWorldPosition()).Normalize());
	}
}

void WeaponManager::DebugGui() {
	if (ImGui::BeginTabItem("WeaponManager")) {
		if (ImGui::TreeNode("PickUpSprite")) {
			samplePickUpWeaponSprite_.DebugGUI();
			ImGui::TreePop();
		}

		// 武器の調整と作成
		if (ImGui::TreeNode("Weapon editor")) {
			// 既存の武器調整
			EditWeaponsGui();
			// 新たに武器の作成
			NewEditWeaponGui();

			ImGui::TreePop();
		}

		// ゲームシーンに武器作成
		if (ImGui::TreeNode("Create Weapon")) {
			CreateWeaponsGui();
			ImGui::TreePop();
		}

		ImGui::EndTabItem();
	}
}

void WeaponManager::CheckPlayerToWeaponDistance() {
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

		//pickUpWeaponLines_.emplace_back();
		int size = (int)pickUpWeaponLines_.size();
		pickUpWeaponLines_.resize(size + 1);
		pickUpWeaponLines_[size].LoadFullPath("resources/model/Weapon/Line.obj");
		pickUpWeaponLines_[size].materials["Material"].color.A = 200;
		pickUpWeaponLines_[size].worldTF.translation = weapon->GetWorldTF()->GetWorldPosition();
		Vector3 dist = (pWorld_->FindActor("Player")->GetModel().GetJointWorldPosition("LockOnAnchor") + Vector3{ 0.0f,-0.5f,0.0f }) - weapon->GetWorldTF()->GetWorldPosition();
		pickUpWeaponLines_[size].worldTF.scale = { 0.1f,0.1f,dist.Length() / 2.0f };
		pickUpWeaponLines_[size].worldTF.rotation = FLMath::LookRotationZLock(dist.Normalize());
		// 回収可能
		que.push(weapon);
	}

	// ***** 回収可能武器がないなら終了 ***** //
	if (que.empty()) { return; }
	// 拾える武器表示
	pickUpWeaponSprite_[que.top()->GetWeaponData().type].isActive = true;
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

void WeaponManager::CreateOriginWeapon() {
	for (int i = 0; i < (int)WeaponType::kCount; i++) {
		sampleWeaponData_[(WeaponType)i];
	}
	for (const auto& [type, weapon] : sampleWeaponData_) {
		// フォルダ指定
		std::string folderName = WeaponConfig::Name::name[(int)type];
		std::vector<std::string> jsonFiles = GetWeaponJsonNames("resources/json/Weapons/" + folderName);
		for (int i = 0; i < jsonFiles.size(); i++) {
			LoadEnemyData((int)type, jsonFiles[i]);
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
		// 使用するモデル名
		.AddValue<std::string>("ModelName", &data.modelName)
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
		// 種類
		.AddValue<int>("Type", &data.type)
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
			bool isClickCombo;
			SelectType(bulletTypePreview_, data.bulletType, "BulletType##0", isClickCombo);
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

void WeaponManager::NewEditWeaponGui() {
	// 新たに武器の作成
	if (ImGui::TreeNode("New weapon")) {
		static int selectModelName = 0;
		bool isClickCombo;
		// 武器種選択
		SelectType(weaponTypePreview_, editWeapon_.type, "WeaponType##101", isClickCombo);
		// タブクリック時の処理
		if (isClickCombo) selectModelName = 0;

		// 武器のモデル選択
		std::vector<std::string> names = GetWeaponModelNames(editWeapon_.type);
		SelectType(names, selectModelName, "WeaponModel##101");
		if (!names.empty()) editWeapon_.modelName = names[selectModelName];

		// 詳細な設定
		WeaponDataGui(editWeapon_);
		ImGui::TreePop();
	}
}

void WeaponManager::EditWeaponsGui() {
	static int selectWeapon = 0;
	static int selectWeaponName = 0;
	static int selectModelName = 0;
	std::vector<std::string> names = GetWeaponNames(selectWeapon);
	std::vector<std::string> modelNames = GetWeaponModelNames(selectWeapon);
	// 既存の武器調整
	if (ImGui::TreeNode("Weapon list")) {
		// 読み込み
		if (!names.empty()) {
			if (ImGui::Button("Load")) {
				LoadEnemyData(selectWeapon, sampleWeaponData_[(WeaponType)selectWeapon][names[selectWeaponName]].jsonFileName);
			}
		}

		// 武器種
		bool isClickCombo;
		SelectType(weaponTypePreview_, selectWeapon, "WeaponType##100", isClickCombo);

		names = GetWeaponNames(selectWeapon);
		// タブクリック時の処理
		if (isClickCombo) {
			// 武器名のプレビュー外を指定しないようにする
			if (!names.empty() && selectWeaponName > (int)names.size() - 1) { selectWeaponName = (int)names.size() - 1; }
			// 指定された武器のモデル名のタブにフォーカスさせる
			for (int i = 0; i < modelNames.size(); i++) {
				if (names.empty()) { break; }
				if (Contains(modelNames[i], sampleWeaponData_[(WeaponType)selectWeapon][names[selectWeaponName]].modelName)) {
					selectModelName = i;
					break;
				}
			}
			// 武器モデルのプレビュー外を指定しないようにする
			modelNames = GetWeaponModelNames(selectWeapon);
			if (!modelNames.empty() && selectModelName > (int)modelNames.size() - 1) { selectModelName = (int)modelNames.size() - 1; }
		}

		if (!names.empty()) {
			// 武器名選択
			SelectType(names, selectWeaponName, "WeaponName##100", isClickCombo);
			// タブクリック時の処理
			if (isClickCombo) {
				// 武器名のプレビュー外を指定しないようにする
				if (!names.empty() && selectWeaponName > (int)names.size() - 1) { selectWeaponName = (int)names.size() - 1; }
				// 指定された武器のモデル名のタブにフォーカスさせる
				for (int i = 0; i < modelNames.size(); i++) {
					if (Contains(modelNames[i], sampleWeaponData_[(WeaponType)selectWeapon][names[selectWeaponName]].modelName)) {
						selectModelName = i;
						break;
					}
				}
			}

			// 武器のモデル選択
			SelectType(modelNames, selectModelName, "WeaponModel##100", isClickCombo);
			if (isClickCombo) {
				sampleWeaponData_[(WeaponType)selectWeapon][names[selectWeaponName]].modelName = modelNames[selectModelName];
			}

			// 詳細な設定
			WeaponDataGui(sampleWeaponData_[(WeaponType)selectWeapon][names[selectWeaponName]]);
		}
		else {
			ImGui::Text("Not found data");
		}
		ImGui::TreePop();
	}
	else {
		for (int i = 0; i < modelNames.size(); i++) {
			if (names.empty()) { break; }

			if (Contains(modelNames[i], sampleWeaponData_[(WeaponType)selectWeapon][names[selectWeaponName]].modelName)) {
				selectModelName = i;
				break;
			}
		}
	}
}

void WeaponManager::CreateWeaponsGui() {
	static int selectWeapon = 0;
	static int selectWeaponName = 0;
	std::vector<std::string> names = GetWeaponNames(selectWeapon);

	// 武器種
	SelectType(weaponTypePreview_, selectWeapon, "WeaponType##2");
	// 武器名
	SelectType(names, selectWeaponName, "WeaponName##2");

	// 生成座標
	ImGui::DragFloat3("CreatePos", &createPos_.x, 0.01f);

	// 武器の四散範囲
	ImGui::DragFloat3("WeaponDropRange", &weaponDropVel.x, 0.01f);
	ImGui::DragFloat("PickUpWeaponAngle", &pickUpWeaponAngle, 0.01f);
	ImGui::DragFloat("PickUpWeaponRange", &pickUpWeaponRange, 0.01f);

	// 武器生成
	if (!names.empty()) {
		if (ImGui::Button("Create")) {
			// コピー元の武器情報
			createWeaponData_ = sampleWeaponData_[(WeaponType)selectWeapon][names[selectWeaponName]];
			IWeapon* weapon = CreateSelectedWeapon(selectWeapon);
			weapons_.push_back(weapon);
			// 武器を地面に出す
			DropWeapon(weapon);
		}
	}

	// 武器解放
	if (ImGui::Button("All Delete")) {
		for (IWeapon* weapon : weapons_) {
			// 所持者がいるなら解放しない
			if (weapon->GetActor()) { continue; }
			DeleteWeapon(weapon);
		}
	}
}

void WeaponManager::SelectType(std::vector<std::string> list, int& selectedType, std::string label, bool& isClickCombo) {
	isClickCombo = false;
	if (!list.empty()) {
		const char* combo_preview_value = list[selectedType].c_str();
		if (ImGui::BeginCombo(label.c_str(), combo_preview_value)) {
			for (int n = 0; n < list.size(); n++) {
				const bool is_selected = ((int)selectedType == n);
				std::string selectableLabel = list[n];
				if (ImGui::Selectable(selectableLabel.c_str(), is_selected)) {
					selectedType = n;
					isClickCombo = true;
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

void WeaponManager::SelectType(std::vector<std::string> list, int& selectedType, std::string label) {
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

std::vector<std::string> WeaponManager::GetWeaponModelNames(int weaponType) {
	std::vector<std::string> modelNamePreview;
	std::vector<std::string> modelNames;
	std::string directoryPath;
	if (weaponType == (int)WeaponType::kMelee) {
		directoryPath = "resources/model/Weapon/Melee/";
		modelNames = GetFileNames((GetExeDir() / directoryPath).string());
	}
	else {
		directoryPath = "resources/model/Weapon/Gun/";
		modelNames = GetFileNames((GetExeDir() / directoryPath).string());
	}

	// exeからのパスは削除
	for (std::string name : modelNames) {
		const std::string target = GetExeDir().string() + "\\";
		size_t pos = name.find(target);
		if (pos != std::string::npos) {
			name.erase(pos, target.length());
		}
		modelNamePreview.push_back(name);
	}
	return modelNamePreview;
}

std::vector<std::string> WeaponManager::GetWeaponModelNames() {
	std::vector<std::string> modelNamePreview;
	std::vector<std::string> modelNames;
	std::string directoryPath = "resources/model/Weapon/";
	modelNames = GetFileNames((GetExeDir().string() + directoryPath));

	// exeからのパスは削除
	for (std::string name : modelNames) {
		const std::string target = GetExeDir().string() + "\\" + "\\";
		size_t pos = name.find(target);
		if (pos != std::string::npos) {
			name.erase(pos, target.length());
		}
		modelNamePreview.push_back(name);
	}
	return modelNamePreview;
}

std::vector<std::string> WeaponManager::GetWeaponJsonNames(const std::string& directoryPath) {
	std::vector<std::string> jsonNamePreview;

	if (!std::filesystem::exists((GetExeDir() / directoryPath).string())) {
		return jsonNamePreview;
	}
	for (const auto& entry : std::filesystem::recursive_directory_iterator((GetExeDir() / directoryPath).string())) {
		if (entry.is_regular_file() &&
			(entry.path().extension() == ".json")) {
			jsonNamePreview.push_back(entry.path().string());
		}
	}

	return jsonNamePreview;
}

std::vector<std::string> WeaponManager::GetWeaponNames(int weaponType) {
	// 対象の配列に何もない
	if (sampleWeaponData_[(WeaponType)weaponType].empty()) { return std::vector<std::string>(); }

	std::vector<std::string> names;
	for (const auto& [name, data] : sampleWeaponData_[(WeaponType)weaponType]) {
		names.push_back(data.name);
	}

	return names;
}

void WeaponManager::WeaponDataGui(WeaponData& data) {
	// 武器名(被りはダメ)
	ImGui::InputText("Weapon Name", &data.name);

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
		SelectType(bulletTypePreview_, data.bulletType, "BulletType##0");
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
	// 武器のレアリティ
	SelectType(weaponRarityPreview_, data.rarity, "RarityType##0");

	if (ImGui::Button("Regist")) {
		sampleWeaponData_[(WeaponType)data.type][data.name] = data;
		ExportEnemyData(data.type, data.name);
	}
}

// ********* jsonファイルの保存で使用する関数↓ ********* //
void WeaponManager::LoadEnemyData(int weaponType, const std::string& fileName) {
	OPENFILENAMEA ofn = { 0 };
	char szFile[MAX_PATH] = { 0 };	// ファイルパスのサイズはWindows既定のものに
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	std::string name = fileName;
	ofn.lpstrFilter = name.c_str();
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST;

	LoadEnemyDataUpdate(weaponType, name);
}

void WeaponManager::LoadEnemyDataUpdate(int weaponType, const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) return;

	nlohmann::json j;
	file >> j;
	WeaponData data{};

	data.name = j["name"].get<std::string>();
	data.type = j["type"];
	data.rarity = j["rarity"];
	data.modelName = j["modelName"].get<std::string>();
	data.jsonFileName = j["jsonFileName"].get<std::string>();

	data.shotIntervalTime = j["shotIntervalTime"];
	data.burstIntervalTime = j["burstIntervalTime"];
	data.bulletNum = j["bulletNum"];
	data.bulletType = j["bulletType"];
	data.sameBulletNum = j["sameBulletNum"];
	data.diffusingBulletRange = {
		j["diffusingBulletRange_x"],
		j["diffusingBulletRange_y"],
		j["diffusingBulletRange_z"]
	};

	data.burstNum = j["burstNum"];
	data.attackSkillGain = j["attackSkillGain"];
	data.storeTime = j["storeTime"];
	data.coolTime = j["coolTime"];
	data.reloadTime = j["reloadTime"];

	sampleWeaponData_[(WeaponType)weaponType][data.name] = data;

	file.close();
}

void WeaponManager::ExportEnemyData(int weaponType, const std::string& weaponName) {
	// ファイル保存ダイアログを使って保存先とファイル名を指定
#if defined(_WIN32)
	char currentDir[MAX_PATH];
	_getcwd(currentDir, MAX_PATH);

	OPENFILENAMEA ofn = { 0 };
	char szFile[MAX_PATH] = { 0 };	// ファイルパスのサイズはWindows既定のものに
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrDefExt = "json";
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

	if (GetSaveFileNameA(&ofn)) {
		ExportEnemyDataUpdate(weaponType, weaponName, szFile);
	}

	_chdir(currentDir);
#endif
}

void WeaponManager::ExportEnemyDataUpdate(int weaponType, const std::string& weaponName, const std::string& fileName) {
	using std::swap;

	const int cJsonIndent = 4;

	nlohmann::json node_json;
	node_json["name"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].name;
	node_json["type"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].type;
	node_json["rarity"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].rarity;
	node_json["modelName"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].modelName;
	sampleWeaponData_[(WeaponType)weaponType][weaponName].jsonFileName = fileName;
	// exeからのパスは削除
	std::string name = sampleWeaponData_[(WeaponType)weaponType][weaponName].jsonFileName;
	const std::string target = GetExeDir().string() + "\\";
	size_t pos = name.find(target);
	if (pos != std::string::npos) {
		name.erase(pos, target.length());
	}
	sampleWeaponData_[(WeaponType)weaponType][weaponName].jsonFileName = name;
	node_json["jsonFileName"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].jsonFileName;

	node_json["shotIntervalTime"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].shotIntervalTime;
	node_json["burstIntervalTime"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].burstIntervalTime;
	node_json["bulletNum"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].bulletNum;
	node_json["bulletType"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].bulletType;
	node_json["sameBulletNum"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].sameBulletNum;

	node_json["diffusingBulletRange_x"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].diffusingBulletRange.x;
	node_json["diffusingBulletRange_y"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].diffusingBulletRange.y;
	node_json["diffusingBulletRange_z"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].diffusingBulletRange.z;

	node_json["burstNum"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].burstNum;
	node_json["attackSkillGain"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].attackSkillGain;
	node_json["storeTime"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].storeTime;
	node_json["coolTime"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].coolTime;
	node_json["reloadTime"] = sampleWeaponData_[(WeaponType)weaponType][weaponName].reloadTime;

	std::ofstream file(fileName);
	if (file.is_open()) {
		file << node_json.dump(cJsonIndent);
		file.close();
	}
}

std::filesystem::path WeaponManager::GetExeDir() {
	return std::filesystem::current_path();
}

std::vector<std::string> WeaponManager::GetFileNames(const std::string& folderPath) {
	std::vector<std::string> result;

	if (!std::filesystem::exists(folderPath)) {
		return result;
	}

	for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) {
		if (entry.is_regular_file() &&
			(entry.path().extension() == ".gltf" || entry.path().extension() == ".obj")) {
			result.push_back(entry.path().string());
		}
	}

	return result;
}
// ********* jsonファイルの保存で使用する関数↑ ********* //

// ********* アクセサ↓ ********* //
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

void WeaponManager::DropWeapon(IWeapon* weapon, const LWP::Math::Vector3& dropPos) {
	if (!weapon) return;

	// 攻撃倍率を初期化
	weapon->SetAttackMultiply(1.0f);

	// 座標指定
	weapon->SetTranslation(dropPos);

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

IWeapon* WeaponManager::CreateWeapon(int weaponType, const std::string& weaponName) {
	if (weaponName == "None") { return nullptr; }

	createWeaponData_ = sampleWeaponData_[(WeaponType)weaponType][weaponName];

	// 選ばれた武器種作成
	IWeapon* weapon = CreateSelectedWeapon(weaponType);
	// 武器リストに追加
	weapons_.push_back(weapon);

	return weapon;
}

IWeapon* WeaponManager::CreateRandomWeapon(const std::vector<int>& weaponTypes, const std::vector<int>& weaponRarity, const std::vector<int>& weaponRarityPercent) {
#pragma region 武器種
	int typeSize = static_cast<int>(weaponTypes.size());
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
	int randomRarity = 0;
	int rarity = 0;

	std::vector<RarityRate> rarityTable = {
	{ RarityType::kCommon,		0 },
	{ RarityType::kUnCommon,	0 },
	{ RarityType::kRare,		0 },
	{ RarityType::kSuperRare,	0 },
	{ RarityType::kLegendary,	0 }
	};
	for (int i = 0; i < weaponRarityPercent.size(); i++) {
		rarityTable[i].weight = weaponRarityPercent[i];
	}
	randomRarity = static_cast<int>(RollRarity(rarityTable));

	rarity = weaponRarity[randomRarity];
#pragma endregion

	// コピー元の武器情報を決定
	std::vector<WeaponData> datas;
	for (const auto& [name, data] : sampleWeaponData_[(WeaponType)type]) {
		if (data.rarity == rarity) datas.push_back(data);
	}
	// 該当レアリティがない
	if (datas.empty()) {
		int diffRarity = 10000;
		int result;
		for (const auto& [name, data] : sampleWeaponData_[(WeaponType)type]) {
			if (static_cast<int>(std::fabsf(data.rarity - rarity)) < diffRarity) {
				diffRarity = static_cast<int>(std::fabsf(data.rarity - rarity));
				result = data.rarity;
			}
		}

		for (const auto& [name, data] : sampleWeaponData_[(WeaponType)type]) {
			if (data.rarity == result) datas.push_back(data);
		}
	}

	int size = static_cast<int>(datas.size());
	int randomWeapon = LWP::Utility::Random::GenerateInt(0, size - 1);

	createWeaponData_ = datas[randomWeapon];

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
// ********* アクセサ↑ ********* //