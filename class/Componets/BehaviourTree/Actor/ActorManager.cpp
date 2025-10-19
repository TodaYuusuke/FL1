#include "ActorManager.h"
#include "Actor.h"
#include <algorithm>
#include <iterator>

//デストラクタ
ActorManager::~ActorManager(){
    Clear();
}

void ActorManager::Update(){
    for (auto actor : pActors_) {
        actor->Update();
    }
}

void ActorManager::DrawGui() const{
    for (auto actor : pActors_) {
        actor->DrawGui();
    }
}

void ActorManager::Add(Actor* actor){
    pActors_.push_back(actor);
}

void ActorManager::Remove(){
    for (auto i = pActors_.begin(); i != pActors_.end();) {
        if (!(*i)->GetIsAlive()) {
            delete* i;
            i = pActors_.erase(i);
        }else {
            ++i;
        }
    }
}

void ActorManager::Clear(){
    for (auto actor : pActors_) {
        delete actor;
    }
    pActors_.clear();
}

Actor* ActorManager::Find(const std::string& name) const{
    auto result = std::find_if(pActors_.begin(), pActors_.end(), 
        [&](Actor* actor) { return actor->GetName() == name; }
    );

    if (result == pActors_.end()) return nullptr;

    return *result;
}

std::vector<Actor*> ActorManager::FindWithTag(const std::string& tag) const{
    std::vector<Actor*> result;

    std::copy_if(pActors_.begin(), pActors_.end(), std::back_inserter(result),
        [&](Actor* actor) { return actor->GetTag() == tag; });

    return result;
}

int ActorManager::Count() const{
    return (int)pActors_.size();
}

int ActorManager::CountWithTag(const std::string& tag) const{
    auto result = std::count_if(pActors_.begin(), pActors_.end(), 
        [&](Actor* actor) { return actor->GetTag() == tag; }
    );

    return static_cast<int>(result);
}
