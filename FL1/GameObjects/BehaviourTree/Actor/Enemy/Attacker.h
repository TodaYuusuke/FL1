#pragma once

#include "../Actor.h"
#include "../IAgent.h"
#include "../ActorHealth.h"
#include "../../CompositeNode/Selector.h"
#include "../../Math/MyTimer.h"
#include "LWP.h"

class IWorld;
class Attacker : public Actor, public IAgent {
	const float cCoolTime = 60.f;

public:
	//コンストラクタ
	Attacker(IWorld* world, LWP::Object::Camera* camera, std::string behavior_tree_file_path);
	//デストラクタ
	~Attacker();
	//更新
	void Update(float delta_time);
	//描画
	void Draw() const;
	//GUIの描画
	void DrawGui();

	// BehaviorTreeの中で現在動いているノードのIDを取得
	const int GetBTRunningNodeID() const;

public:
	const LWP::Math::Vector3& GetPos() const override;
	void MoveTowards(const LWP::Math::Vector3& target, float speed) override;

	const float GetHealth() const override;
	void Attack() override;

private:
	void Move(float delta_time);

private:
	INode* behaviourTree = nullptr;
};
