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
	body_.LoadSphere();

	// マガジン作成
	magazine_ = std::make_unique<Magazine>(gunData_.magazineModelName, gunData_.bulletNum);

	Init();
}

void Gun::Init() {
	body_.worldTF.translation = { 0.0f,0.0f,0.0f };
	body_.worldTF.rotation = { 0.0f,0.0f,0.0f,1.0f };
	body_.worldTF.scale = { 1.0f,1.0f,1.0f };

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
		Reload();
	}

	shotFrame_--;

	shotFrame_ = std::max<float>(shotFrame_, 0.0f);
	reloadFrame_ = std::max<float>(reloadFrame_, 0.0f);
}

void Gun::DebugGui() {

}

void Gun::Attack() {
	// 弾がない状態なら撃てない
	if (magazine_->GetEmpty()) { return; }
	// 射撃できる状態か
	if (!GetIsEnableShot()) { return; }

	// 弾を撃つ
	Bullet* bullet = new Bullet(body_.worldTF.GetWorldPosition(), GetDirVector({ 0,0,1 }, character_->GetModel().worldTF.rotation));
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
