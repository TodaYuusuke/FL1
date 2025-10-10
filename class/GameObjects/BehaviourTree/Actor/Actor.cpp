#include "Actor.h"

void Actor::Init() {}

void Actor::Update(){}

void Actor::DrawGui() {}

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