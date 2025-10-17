#include "IMelee.h"

using namespace FLMath;

IMelee::IMelee(GunData gunData) {
	// 調整項目を代入
	gunData_ = gunData;
	name_ = gunData_.gunModelName;

	// モデル生成
	body_.LoadFullPath("resources/model/" + gunData_.gunModelName);
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン作成
	magazine_ = std::make_unique<Magazine>(gunData_.magazineModelName, gunData_.bulletNum);

	Init();
}

void IMelee::Init() {
	body_.worldTF.translation = { 0.0f,0.0f,0.0f };
	body_.worldTF.rotation = { 0.0f,0.0f,0.0f,1.0f };
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン初期化
	magazine_->Init(gunData_.bulletNum);

	// 攻撃力
	currentAttackValue_ = gunData_.attackValue;

	// 射撃時の経過時間
	shotFrame_ = gunData_.shotIntervalTime * 60.0f;
	// リロードの経過時間
	reloadFrame_ = gunData_.reloadTime * 60.0f;
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
	if (ImGui::TreeNode("Magazine")) {
		magazine_->DebugGui();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("CoolTime")) {
		ImGui::DragFloat("ShotFrame", &shotFrame_);
		ImGui::DragFloat("ReloadFrame", &reloadFrame_);
		ImGui::TreePop();
	}
}

void IMelee::Attack() {
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
	shotFrame_ = gunData_.shotIntervalTime * 60.0f;
}

void IMelee::Reload() {
	reloadFrame_--;

	// リロード完了
	if (!GetIsReloading()) {
		// リロード時間を初期化
		reloadFrame_ = gunData_.reloadTime * 60.0f;
		// 弾数を初期化
		magazine_->Init(gunData_.bulletNum);
	}
}

void IMelee::Destroy() {

}