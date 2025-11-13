#include "World.h"
#include "Scene/IScene.h"
#include "../../GameObjects/Bullets/BulletManager.h"
#include "../../GameObjects/Weapon/WeaponManager.h"
#include "../../Componets/BehaviourTree/Actor/Actor.h"

//デストラクタ
World::~World() {
	Clear();
}

//更新
void World::Update() {
	//アクターの更新
	actorManager.Update();
	//アクターの消去
	actorManager.Remove();
}

void World::DebugGui() {
	if (ImGui::BeginTabItem("World")) {
		//GUIの描画
		actorManager.DrawGui();
		ImGui::EndTabItem();
	}
}

//消去
void World::Clear() {
	//アクターの消去
	actorManager.Clear();
}

//アクターの追加
void World::AddActor(Actor* actor) {
	actorManager.Add(actor);
}

//アクターの検索
Actor* World::FindActor(const std::string& name) const {
	return actorManager.Find(name);
}

//指定したタグ名を持つアクターの検索
std::vector<Actor*> World::FindActorWithTag(const std::string& tag) const {
	return actorManager.FindWithTag(tag);
}

//アクター数を返す
int World::CountActor() const {
	return actorManager.Count();
}

//指定したタグ名を持つアクター数を返す
int World::CountActorWithTag(const std::string& tag) const {
	return actorManager.CountWithTag(tag);
}

float World::FindAttackPower(const std::string& name) {
	// 自機、障害物オブジェクト
	if (actorManager.Find(name)) {
		return actorManager.Find(name)->GetAttackPower();
	}
	
	return BulletManager::GetInstance()->FindBullet(name)->GetAttackPower();
}
