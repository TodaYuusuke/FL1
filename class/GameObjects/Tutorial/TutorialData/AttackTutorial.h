#pragma once
#include <Adapter.h>
#include "BaseTutorialData.h"
#include "../../Weapon/WeaponManager.h"
#include "../../../Componets/Input/VirtualController.h"

class Player;
class EnemyManager;
class AttackTutorial : public BaseTutorialData {
public:
	enum class AttackGuideSequence {
		kIntro,
		kLeftAttack,
		kRightAttack,
		kLeftShoulderAttack,
		kRightShoulderAttack,
		kKill,
		kCount
	};

public:
	// コンストラクタ
	AttackTutorial(Player* player, EnemyManager* enemyManager);
	// デストラクタ
	~AttackTutorial() override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
	/// <summary>
	/// 成功演出
	/// </summary>
	/// <param name="echoSprite"></param>
	void SuccessEffect(AttackGuideSequence target);

private:
	// 仮想コントローラ
	VirtualController* vCon_;

private:
	// 
	std::map<AttackGuideSequence, LWP::Primitive::NormalSprite> guideFont_;
	std::map<AttackGuideSequence, LWP::Primitive::NormalSprite> echoFont_;

	// チュートリアルの順序
	AttackGuideSequence sequence_;

	// 
	LWP::Math::Vector3 centerPos_;
};

