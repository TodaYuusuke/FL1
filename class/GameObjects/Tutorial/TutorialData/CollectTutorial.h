#pragma once
#include <Adapter.h>
#include "BaseTutorialData.h"
#include "../../Weapon/WeaponManager.h"
#include "../../../Componets/Input/VirtualController.h"

class Player;
class EnemyManager;
class CollectTutorial : public BaseTutorialData {
public:
	enum class CollectGuideSequence {
		kIntro,
		kCollect,
		kCount
	};

public:
	// コンストラクタ
	CollectTutorial(Player* player, EnemyManager* enemyManager);
	// デストラクタ
	~CollectTutorial() override = default;

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
	void SuccessEffect(CollectGuideSequence target);

private:
	// 仮想コントローラ
	VirtualController* vCon_;
	// 武器管理クラス
	WeaponManager* weaponManager_;

private:
	// 
	std::map<CollectGuideSequence, LWP::Primitive::NormalSprite> guideFont_;
	std::map<CollectGuideSequence, LWP::Primitive::NormalSprite> echoFont_;

	// 落ちる武器
	std::array<int, 2> dropWeapons_;

	// チュートリアルの順序
	CollectGuideSequence sequence_;

	// 
	LWP::Math::Vector3 centerPos_;
};

