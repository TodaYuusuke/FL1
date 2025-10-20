#pragma once
#include "../../../Componets/BehaviourTree/Actor/ActorHealth.h"
#include "../../../Componets/BehaviourTree/BehaviorTreeUtility.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/BehaviourTree/INode.h"
#include <Adapter.h>

class IWorld;
class Gunner : public Actor {
public:
	//コンストラクタ
	Gunner(IWorld* world, int ID, const std::string& BTFilePath);
	//デストラクタ
	~Gunner() override;

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

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack() override;

	// BehaviorTreeの中で現在動いているノードのIDを取得
	const int GetBTRunningNodeID() const;

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

private:
	// 識別番号
	int ID_;
};
