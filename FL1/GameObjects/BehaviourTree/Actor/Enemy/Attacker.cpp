#include "Attacker.h"

#include "../BlackBoard.h"
#include "../../../World/IWorld.h"
#include "../../BehaviourTreeBulider.h"
#include "../../CompositeNode/Sequence.h"
#include "../../DecoratorNode/Inverter.h"
#include "../../LeafNode/AlwaysSuccessLeaf.h"
#include "../../LeafNode/ChasePlayerLeaf.h"
#include "../../LeafNode/WaitLeaf.h"
#include "../../LeafNode/CircleAttackLeaf.h"
#include "../../BranchNode/CheckNearPlayer.h"

using namespace LWP::Math;

Attacker::Attacker(IWorld* world, LWP::Object::Camera* camera, std::string behavior_tree_file_path) {
	this->world = world;
	tag = "EnemyTag";
	name = "Attacker";
	attackPower = 1;

	blackBoard = new BlackBoard();
	blackBoard->SetValue<IAgent*>("Agent", this);

	auto* p_player = world->FindActor("Player");
	if (p_player == nullptr) {
		return;
	}
	Vector3 pos = p_player->GetPos();
	blackBoard->SetValue<Vector3>("PlayerPos", pos);

	behaviourTree = BehaviourTreeBuilder::BuildAttackerTree("Engine/resources/behavior_tree.json", blackBoard);
	behaviourTree->Init();

	Vector3 min = pos - Vector3{ 30.f, 30.f };
	Vector3 max = pos + Vector3{ 30.f, 30.f };
}

Attacker::~Attacker()
{
	delete behaviourTree;
}

void Attacker::Update(float delta_time)
{
	auto* p_player = world->FindActor("Player");

	if (p_player == nullptr) {
		return;
	}

	Vector3 pos = p_player->GetPos();
	blackBoard->SetValue<Vector3>("PlayerPos", pos);

	behaviourTree->Tick();

	Move(delta_time);

	velocity = Vector3{ 0.0f, 0.0f, 0.0f };
}

void Attacker::Draw() const
{
	//object_->Draw();
	//DrawCircle(mPosition.x, mPosition.y, 32, GetColor(255, 0, 0), TRUE);
	//mHealthBar.draw();
}

void Attacker::DrawGui() {
	//mCollider.draw_debug();
}

const int Attacker::GetBTRunningNodeID() const
{
	return behaviourTree->GetRunningNodeID();
}

const Vector3& Attacker::GetPos() const
{
	return pos;
}

void Attacker::MoveTowards(const Vector3& target, float speed)
{
	velocity = target * speed;
}

const float Attacker::GetHealth() const
{
	return health.GetHealth();
}

void Attacker::Attack() {
	//Vector2 min, max;

	//min = mPosition - Vector2{ 75.f, 75.f };
	//max = mPosition + Vector2{ 75.f, 75.f };
	//MyRectangle attack_collider{ min, max };

	//mpWorld->add_actor(new AttackCollider{ mpWorld,attack_collider, mTag, 15.f , 10.f });
}

void Attacker::Move(float delta_time)
{
	Vector3 velocity = velocity * delta_time;
	pos += velocity;
}