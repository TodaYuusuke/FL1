#include "IGun.h"
#include "../../Bullets/BulletManager.h"
#include "../../Bullets/Bullet/Bullet.h"

using namespace FLMath;
using namespace LWP::Math;

IGun::IGun(WeaponData gunData) {
	// 弾管理クラスのアドレス取得
	pBulletManager_ = BulletManager::GetInstance();

	// 調整項目を代入
	gunData_ = gunData;
	name_ = gunData_.modelName;

	// モデル生成
	body_.LoadFullPath("resources/model/" + gunData_.modelName);
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン作成
	magazine_ = std::make_unique<Magazine>(gunData_.bulletNum);

	Init();
}

void IGun::Init() {
	body_.worldTF.translation = { 0.0f,0.0f,0.0f };
	body_.worldTF.rotation = { 0.0f,0.0f,0.0f,1.0f };
	body_.worldTF.scale = { 0.5f,0.5f,0.5f };

	// マガジン初期化
	magazine_->Init(gunData_.bulletNum);

	// 攻撃力
	currentAttackValue_ = gunData_.attackValue;

	// 射撃時の経過時間
	attackFrame_ = gunData_.shotIntervalTime * 60.0f;
	// リロードの経過時間
	coolFrame_ = gunData_.coolTime * 60.0f;
}

void IGun::Update() {
	// 弾がなくなれば強制リロード(クールタイム)
	if (magazine_->GetEmpty()) {
		isDestroy_ = true;
		Reload();
	}

	attackFrame_--;

	attackFrame_ = std::max<float>(attackFrame_, 0.0f);
	coolFrame_ = std::max<float>(coolFrame_, 0.0f);
}

void IGun::DebugGui() {
	if (ImGui::TreeNode("Magazine")) {
		magazine_->DebugGui();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("CoolTime")) {
		ImGui::DragFloat("ShotFrame", &attackFrame_);
		ImGui::DragFloat("ReloadFrame", &coolFrame_);
		ImGui::TreePop();
	}
}

void IGun::Attack() {
	// 弾がない状態なら撃てない
	if (magazine_->GetEmpty()) { 
		isDestroy_ = true;
		return; 
	}
	// 射撃できる状態か
	if (!GetIsEnableAttack()) { return; }

	// 弾を撃つ
	//Bullet* bullet = new Bullet(body_.worldTF.GetWorldPosition(), GetDirVector({ 0,0,1 }, actor_->GetModel().worldTF.rotation));
	Bullet* bullet = new Bullet(body_.worldTF.GetWorldPosition(), shotDirVel_ * 1.0f);
	pBulletManager_->CreateBullet(bullet);

	// 弾数を減らす
	magazine_->BulletDecrement();

	// 射撃間隔を初期化
	attackFrame_ = gunData_.shotIntervalTime * 60.0f;
}

void IGun::Reload() {
	coolFrame_--;

	// リロード完了
	if (!GetIsCoolTime()) {
		// リロード時間を初期化
		coolFrame_ = gunData_.coolTime * 60.0f;
		// 弾数を初期化
		magazine_->Init(gunData_.bulletNum);
	}
}

void IGun::Destroy() {

}