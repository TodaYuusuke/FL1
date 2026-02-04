#pragma once
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../Weapon/WeaponSlot.h"
#include "../EnemyConfig.h"
#include "../../Player/Anim/RobotAnimManager.h"
#include <Adapter.h>

class IWorld;
class MeleeAttacker : public Actor {
public:
	//コンストラクタ
	MeleeAttacker(IWorld* world, int ID, const EnemyData& data);
	//デストラクタ
	~MeleeAttacker() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	//void Update() override;
	/// <summary>
	/// 調整項目
	/// </summary>
	void DrawGui() override;

	// BehaviorTreeの中で現在動いているノードのIDを取得
	const int GetBTRunningNodeID() const;

private:
	void Move();

	/// <summary>
	/// アニメーションマネージャーの更新関数
	/// </summary>
	void AnimManagerUpdate() override;

public: // 演出用関数群

	/// <summary>
	/// 射撃アニメーション再生関数
	/// </summary>
	/// <param name="weaponSide">武器の場所</param>
	void PlayAttackAnim(const int weaponSide = 0) override;


private:
	// 武器リスト
	std::map<int, IWeapon*> weapons_;

	// 識別番号
	int ID_;

	// アニメーションマネージャー
	std::unique_ptr<RobotAnimManager> animManager_;

};
