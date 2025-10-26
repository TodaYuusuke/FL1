#include "IMelee.h"

using namespace FLMath;

IMelee::IMelee(WeaponData data) {
	// 調整項目を代入
	data_ = data;
	name_ = data_.modelName;

	// モデル生成
	body_.LoadFullPath("resources/model/Weapon/Melee/" + data_.modelName);
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン作成
	magazine_ = std::make_unique<Magazine>(data_.bulletNum);

	Init();
}

void IMelee::Init() {
	body_.worldTF.translation = { 0.0f,0.0f,0.0f };
	body_.worldTF.rotation = { 0.0f,0.0f,0.0f,1.0f };
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン初期化
	magazine_->Init(data_.bulletNum);

	// 攻撃力
	currentAttackValue_ = data_.attackValue;

	// 射撃時の経過時間
	shotFrame_ = data_.shotIntervalTime * 60.0f;
	// リロードの経過時間
	reloadFrame_ = data_.coolTime * 60.0f;
}

void IMelee::Update() {
	// 弾がなくなれば強制リロード(クールタイム)
	if (magazine_->GetEmpty()) {
		isDestroy_ = true;
	}

	shotFrame_--;

	shotFrame_ = std::max<float>(shotFrame_, 0.0f);
	reloadFrame_ = std::max<float>(reloadFrame_, 0.0f);
}

void IMelee::DebugGui() {
	// 調整項目
	if (ImGui::TreeNode("Json")) {
		if (ImGui::Button("Save")) {
			json_.Save();
		}
		if (ImGui::Button("Load")) {
			json_.Load();
		}

		// 発射間隔
		if (ImGui::TreeNode("Interval")) {
			ImGui::DragFloat("Normal", &data_.shotIntervalTime);
			ImGui::DragFloat("Burst", &data_.burstIntervalTime);
			ImGui::TreePop();
		}
		// 弾
		if (ImGui::TreeNode("Bullet")) {
			ImGui::DragFloat("Num", &data_.bulletNum);
			ImGui::DragFloat("Speed", &data_.bulletSpeed);
			ImGui::TreePop();
		}
		// 溜め時間
		ImGui::DragFloat("Store", &data_.storeTime);
		// 攻撃力
		ImGui::DragFloat("AttackPower", &data_.attackValue);
		// 撃てない時間
		ImGui::DragFloat("CoolTime", &data_.coolTime);
		// レアリティ
		ImGui::DragInt("Rarity", &data_.rarity);

		ImGui::TreePop();
	}

	// 現在の情報
	if (ImGui::TreeNode("CurrentData")) {
		if (ImGui::TreeNode("Energy")) {
			magazine_->DebugGui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("CoolTime")) {
			ImGui::DragFloat("AttackFrame", &attackFrame_);
			ImGui::DragFloat("CoolFrame", &coolFrame_);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void IMelee::CreateJsonData(const std::string& name) {
	// モデル非表示
	body_.isActive = false;

	// ファイル名
	std::string fileName = name + ".json";
	json_.Init(fileName)
		// 発射間隔
		.BeginGroup("Interval")
		.AddValue<float>("Normal", &data_.shotIntervalTime)
		.AddValue<float>("Burst", &data_.burstIntervalTime)
		.EndGroup()
		// 弾
		.BeginGroup("Bullet")
		.AddValue<float>("Num", &data_.bulletNum)
		.AddValue<float>("Speed", &data_.bulletSpeed)
		.EndGroup()
		// 溜め時間
		.AddValue<float>("Store", &data_.storeTime)
		// 攻撃力
		.AddValue<float>("AttackPower", &data_.attackValue)
		// 撃てない時間
		.AddValue<float>("CoolTime", &data_.coolTime)
		// レアリティ
		.AddValue<int>("Rarity", &data_.rarity)
		.CheckJsonFile();
}

void IMelee::Attack(int bulletHitFragBit) {
	bulletHitFragBit;

	// 弾がない状態なら撃てない
	if (magazine_->GetEmpty()) { 
		isDestroy_ = true;
		return; 
	}
	// 射撃できる状態か
	if (!GetIsEnableShot()) { return; }

	// 弾数を減らす
	magazine_->BulletDecrement();

	// 射撃間隔を初期化
	shotFrame_ = data_.shotIntervalTime * 60.0f;
}

void IMelee::Reload() {
	reloadFrame_--;

	// リロード完了
	if (!GetIsReloading()) {
		// リロード時間を初期化
		reloadFrame_ = data_.coolTime * 60.0f;
		// 弾数を初期化
		magazine_->Init(data_.bulletNum);
	}
}

void IMelee::Destroy() {

}