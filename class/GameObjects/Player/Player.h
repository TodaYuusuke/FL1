#pragma once
#include "System/LeadingSystem.h" 
#include "Actions/MoveController.h"
#include "Actions/WeaponController.h"
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include <Adapter.h>

class EnemyManager;
/// <summary>
/// 自機クラス
/// </summary>
class Player : public Actor {
public:
	// コンストラクタ
	Player(LWP::Object::Camera* camera);
	// デストラクタ
	~Player() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 調整項目
	/// </summary>
	void DrawGui() override;

public:// アクセサ
#pragma region Getter
	
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 武器を設定
	/// </summary>
	/// <returns></returns>
	void SetWeapon(IWeapon* weapon, int weaponSide) override {
		weaponSide;
		weaponController_->SetWeapon(weapon); 
	}
	/// <summary>
	/// 左側の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetLeftWeapon(IWeapon* weapon) { weaponController_->SetLeftWeapon(weapon); }
	/// <summary>
	/// 右側の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightWeapon(IWeapon* weapon) { weaponController_->SetRightWeapon(weapon); }
	/// <summary>
	/// 敵管理クラスのアドレスを設定
	/// </summary>
	/// <param name="enemyManager"></param>
	void SetEnemyManager(EnemyManager* enemyManager) { 
		pEnemyManager_ = enemyManager;
		// 偏差射撃機能にも設定
		leadingSystem_->SetEnemyManager(enemyManager);
	}
#pragma endregion

private:// 外部から受け取る変数
	// 敵管理クラス
	EnemyManager* pEnemyManager_;

private:
	// 偏差射撃機能
	std::unique_ptr<LeadingSystem> leadingSystem_;

	// 移動行動系の処理
	std::unique_ptr<MoveController> moveController_;
	// 武器の処理
	std::unique_ptr<WeaponController> weaponController_;
};