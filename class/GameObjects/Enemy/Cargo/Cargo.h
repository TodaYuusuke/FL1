#pragma once
#include "../../../Componets/BehaviourTree/Actor/Health.h"
#include "../../../Componets/BehaviourTree/BehaviorTreeUtility.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/BehaviourTree/INode.h"
#include "../../Weapon/WeaponSlot.h"
#include "../EnemyConfig.h"
#include <Adapter.h>

class IWorld;
/// <summary>
/// 輸送機(常に逃げる)
/// </summary>
class Cargo : public Actor {
public:
	//コンストラクタ
	Cargo(IWorld* world, int ID, const EnemyData& data);
	//デストラクタ
	~Cargo() override;

	// BehaviorTreeの中で現在動いているノードのIDを取得
	const int GetBTRunningNodeID() const;

private:
	// 武器リスト
	std::map<int, IWeapon*> weapons_;

	// 識別番号
	int ID_;
};