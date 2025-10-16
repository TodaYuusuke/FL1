#include "Gun.h"
#include "../../Bullets/BulletManager.h"
#include "../../Bullets/Bullet/Bullet.h"

using namespace FLMath;

Gun::Gun(GunData gunData) {
	// 弾管理クラスのアドレス取得
	pBulletManager_ = BulletManager::GetInstance();

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

void Gun::Init() {
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

void Gun::Update() {
	// 弾がなくなれば強制リロード(クールタイム)
	if (magazine_->GetEmpty()) {
		isDestroy_ = true;
		//Reload();
	}

	shotFrame_--;

	shotFrame_ = std::max<float>(shotFrame_, 0.0f);
	reloadFrame_ = std::max<float>(reloadFrame_, 0.0f);
}

void Gun::DebugGui() {
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

void Gun::Attack() {
	// 弾がない状態なら撃てない
	if (magazine_->GetEmpty()) { 
		isDestroy_ = true;
		return; 
	}
	// 射撃できる状態か
	if (!GetIsEnableShot()) { return; }

	// 弾を撃つ
	Bullet* bullet = new Bullet(body_.worldTF.GetWorldPosition(), GetDirVector({ 0,0,1 }, actor_->GetModel().worldTF.rotation));
	pBulletManager_->CreateBullet(bullet);

	// 弾数を減らす
	magazine_->BulletDecrement();

	// 射撃間隔を初期化
	shotFrame_ = gunData_.shotIntervalTime * 60.0f;
}

void Gun::Reload() {
	reloadFrame_--;

	// リロード完了
	if (!GetIsReloading()) {
		// リロード時間を初期化
		reloadFrame_ = gunData_.reloadTime * 60.0f;
		// 弾数を初期化
		magazine_->Init(gunData_.bulletNum);
	}
}
