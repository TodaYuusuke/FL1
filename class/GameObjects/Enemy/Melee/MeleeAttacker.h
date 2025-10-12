#pragma once
#include "../../../Componets/BehaviourTree/Actor/ActorHealth.h"
#include "../../../Componets/BehaviourTree/BehaviorTreeUtility.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/BehaviourTree/INode.h"
#include <Adapter.h>

class IWorld;
class MeleeAttacker : public Actor {
public:
	//コンストラクタ
	MeleeAttacker(IWorld* world, LWP::Object::Camera* camera, std::string behavior_tree_file_path);
	//デストラクタ
	~MeleeAttacker() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 調整項目
	/// </summary>
	void DrawGui() override;

	// BehaviorTreeの中で現在動いているノードのIDを取得
	const int GetBTRunningNodeID() const;

	//void Attack() override;

private:
	void Move();

private:
	// ビヘイビアツリー
	INode* bt_ = nullptr;
	// ビヘイビアツリーの編集
	std::unique_ptr<BehaviorTreeGraph> btEditor_;
};
