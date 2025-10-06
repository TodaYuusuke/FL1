#include "World.h"
#include "Scene/IScene.h"
#include "../BehaviourTree/Actor/Actor.h"

//デストラクタ
World::~World() {
	Clear();
}

//更新
void World::Update(float delta_time) {
	//アクターの更新
	actorManager.Update(delta_time);
	//アクターの消去
	actorManager.Remove();
}

//描画
void World::Draw() const {
	//フィールド描画
	//mpField->draw();
	//アクターの描画
	actorManager.Draw();
	//GUIの描画
	actorManager.DrawGui();
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

//メッセージ送信
void World::Send_Message(const std::string& message, void* param) {
	actorManager.Send_Message(message, param);
}