#include "World.h"
#include "Scene/IScene.h"
#include "../../GameObjects/Attack/AttackManager.h"
#include "../../GameObjects/Weapon/WeaponManager.h"
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include <numbers>

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Primitive;

World::~World() {
	Clear();
}

World::World() {
	skydome_.LoadShortPath("level/Skydome/skydome.gltf");
	skydome_.SetAllMaterialLighting(false);
	skydome_.worldTF.scale = { 1000.0f,1000.0f,1000.0f };

	json_.Init("Game.json")
		.AddValue<Vector3>("MoveArea", &moveArea)

		.BeginGroup("Surface")
		.AddValue<Vector3>("MoveLimitScale", &scale_)
		.AddValue<float>("MoveLimitSpeed", &moveLimitSpeed_)
		.EndGroup()

		.CheckJsonFile();

	float pi = static_cast<float>(std::numbers::pi);
	moveLimitSurface_["Front"].worldTF.translation = { 0.0f,8.0f,moveArea.z };
	moveLimitSurface_["Back"].worldTF.translation = { 0.0f,8.0f,-moveArea.z };
	moveLimitSurface_["Back"].worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, pi);
	moveLimitSurface_["Right"].worldTF.translation = { moveArea.x,8.0f,0.0f };
	moveLimitSurface_["Right"].worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, pi / 2.0f);
	moveLimitSurface_["Left"].worldTF.translation = { -moveArea.x,8.0f,0.0f };
	moveLimitSurface_["Left"].worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3{ 0,1,0 }, -pi / 2.0f);

	scale_ = { 46.875f,10.0f,1.0f };
	for (auto& [key, surface] : moveLimitSurface_) {
		surface.LoadTexture("alertBelt.png");
		surface.anchorPoint = { 0.5f, 0.5f };
		surface.worldTF.scale = scale_;
	}
}

void World::Update() {
	//アクターの更新
	actorManager.Update();
	//アクターの消去
	//actorManager.Remove();

	for (auto& [key, surface] : moveLimitSurface_) {
		surface.material.uvTransform.translation.x += moveLimitSpeed_;
		surface.worldTF.scale = scale_;
	}
}

void World::DebugGui() {
	if (ImGui::BeginTabItem("World")) {
		json_.DebugGUI();
		if (std::fabsf(moveArea.x) <= 10.0f) {
			moveArea.x = 10.0f;
		}
		if (std::fabsf(moveArea.y) <= 10.0f) {
			moveArea.y = 10.0f;
		}
		if (std::fabsf(moveArea.z) <= 10.0f) {
			moveArea.z = 10.0f;
		}

		//GUIの描画
		actorManager.DrawGui();
		if (ImGui::TreeNode("Skydome")) {
			skydome_.DebugGUI();
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}
}

void World::Clear() {
	//アクターの消去
	actorManager.Clear();
}

void World::LimitMoveArea(LWP::Math::Vector3& translation) {
	translation.x = std::clamp<float>(translation.x, -moveArea.x + 20.0f, moveArea.x - 20.0f);
	translation.y = std::clamp<float>(translation.y, -moveArea.y + 20.0f, moveArea.y - 20.0f);
	translation.z = std::clamp<float>(translation.z, -moveArea.z + 20.0f, moveArea.z - 20.0f);
}

bool World::GetIsLimitMoveArea(const LWP::Math::Vector3& translation) {
	if (std::fabsf(translation.x) >= moveArea.x ||
		std::fabsf(translation.y) >= moveArea.y ||
		std::fabsf(translation.z) >= moveArea.z) {
		return true;
	}

	return false;
}

void World::AddActor(Actor* actor) {
	actorManager.Add(actor);
}

Actor* World::FindActor(const std::string& name) const {
	return actorManager.Find(name);
}

std::vector<Actor*> World::FindActorWithTag(const std::string& tag) const {
	return actorManager.FindWithTag(tag);
}

int World::CountActor() const {
	return actorManager.Count();
}

int World::CountActorWithTag(const std::string& tag) const {
	return actorManager.CountWithTag(tag);
}

float World::FindAttackPower(const std::string& name) {
	// 自機、障害物オブジェクト
	if (actorManager.Find(name)) {
		return actorManager.Find(name)->GetAttackPower();
	}

	return AttackManager::GetInstance()->FindBullet(name)->GetAttackPower();
}
