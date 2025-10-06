#include "Actor.h"

void Actor::Update(float delta_time){}

void Actor::Draw() const{}

void Actor::DrawGui() {}

void Actor::HandleMessage(const std::string& message, void* param){}

void Actor::Die(){
    isDead = true;
}

bool Actor::GetIsDead() const{
    return isDead;
}

const std::string& Actor::GetName() const{
    return name;
}

const std::string& Actor::GetTag() const{
    return tag;
}

const LWP::Math::Vector3& Actor::GetPos() const {
    return pos;
}

float Actor::GetAttackPower() const{
    return attackPower;
}