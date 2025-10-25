#include "BulletManager.h"

BulletManager::BulletManager() {

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

void BulletManager::CreateBullet(BulletBase* bullet) {
	bullets_.push_back(bullet);
}