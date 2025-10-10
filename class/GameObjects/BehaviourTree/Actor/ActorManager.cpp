#include "ActorManager.h"
#include "Actor.h"

#include <algorithm>
#include <iterator>

//デストラクタ
ActorManager::~ActorManager(){
    Clear();
}

//アクターの追加
void ActorManager::Add(Actor* actor){
    pActors.push_back(actor);
}

//アクターの更新
void ActorManager::Update(){
    for (auto actor : pActors) {
        actor->Update();
    }
}

//アクターのGUI描画
void ActorManager::DrawGui() const{
    for (auto actor : pActors) {
        actor->DrawGui();
    }
}

//死亡しているアクターの削除
void ActorManager::Remove(){
    for (auto i = pActors.begin(); i != pActors.end();) {
        if ((*i)->GetIsDead()) {
            delete* i;
            i = pActors.erase(i);
        }else {
            ++i;
        }
    }
}

//アクターの検索
Actor* ActorManager::Find(const std::string& name) const{
    auto result = std::find_if(
        pActors.begin(), pActors.end(), 
        [&](Actor* actor) { return actor->GetName() == name; }
    );

    if (result == pActors.end()) return nullptr;

    return *result;
}

//指定したタグ名を持つアクターの検索
std::vector<Actor*> ActorManager::FindWithTag(const std::string& tag) const{
    std::vector<Actor*> result;

    std::copy_if(pActors.begin(), pActors.end(), std::back_inserter(result),
        [&](Actor* actor) { return actor->GetTag() == tag; });

    return result;
}

//アクター数を返す
int ActorManager::Count() const{
    return (int)pActors.size();
}

//指定したタグのアクター数を返す
int ActorManager::CountWithTag(const std::string& tag) const{
    auto result = std::count_if(
        pActors.begin(), pActors.end(), 
        [&](Actor* actor) { return actor->GetTag() == tag; }
    );

    return static_cast<int>(result);
}

//消去
void ActorManager::Clear(){
    for (auto actor : pActors) {
        delete actor;
    }
    pActors.clear();
}