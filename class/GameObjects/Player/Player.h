#pragma once
#include "System/LeadingSystem.h" 
#include "Actions/MoveController.h"
#include "Actions/WeaponController.h"
#include "System/WeaponSkill/WeaponSkill.h"
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include "../Camera/FollowCamera/FollowCamera.h"
#include <Adapter.h>

class EnemyManager;
/// <summary>
/// 自機クラス
/// </summary>
class Player : public Actor {
public:
	// コンストラクタ
	Player(FollowCamera* camera, IWorld* world, const LWP::Math::Vector3& centerPos);
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

private:// プライベートな関数
	void AdjustRotate();

	/// <summary>
	/// 左手の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetLeftWeapon(IWeapon* weapon) { weaponController_->SetLeftWeapon(weapon); }
	/// <summary>
	/// 右手の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightWeapon(IWeapon* weapon) { weaponController_->SetRightWeapon(weapon); }
	/// <summary>
	/// 左肩の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetLeftShoulderWeapon(IWeapon* weapon) { weaponController_->SetLeftShoulderWeapon(weapon); }
	/// <summary>
	/// 右肩の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightShoulderWeapon(IWeapon* weapon) { weaponController_->SetRightShoulderWeapon(weapon); }
	/// <summary>
	/// 武器の選択
	/// </summary>
	/// <param name="selectedWeaponType"></param>
	/// <param name="label"></param>
	void SelectWeaponType(int& selectedWeaponType, std::string label);
	/// <summary>
	/// 武器のレアリティ選択
	/// </summary>
	/// <param name="selectedWeaponRarity"></param>
	/// <param name="label"></param>
	void SelectWeaponRarity(int& selectedWeaponRarity, std::string label);

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetCenterPosition() override;
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
	/// 敵管理クラスのアドレスを設定
	/// </summary>
	/// <param name="enemyManager"></param>
	void SetEnemyManager(EnemyManager* enemyManager) { 
		pEnemyManager_ = enemyManager;
		// 偏差射撃機能にも設定
		leadingSystem_->SetEnemyManager(enemyManager);
	}
	/// <summary>
	/// 中心座標を設定
	/// </summary>
	/// <param name="centerPos"></param>
	void SetCenterDist(const LWP::Math::Vector3& centerPos) { centerPos_ = centerPos; }
#pragma endregion

private:// 外部から受け取る変数
	// 敵管理クラス
	EnemyManager* pEnemyManager_;
	// 中心座標
	LWP::Math::Vector3 centerPos_;

private:// デバッグ用変数
	// 作成できる武器種一覧
	std::vector<std::string> weaponTypePreview_;
	// 作成できる武器のレアリティ一覧
	std::vector<std::string> weaponRarityPreview_;
	// 初期武器
	std::array<int, (int)WeaponSide::kCount> weaponTypies_;
	std::array<int, (int)WeaponSide::kCount> weaponRarities_;

private:
	float maxHp_ = 100.0f;

private:
	LWP::Utility::JsonIO json_;

	// 偏差射撃機能
	std::unique_ptr<LeadingSystem> leadingSystem_;

	// 移動行動系の処理
	std::unique_ptr<MoveController> moveController_;
	// 武器の処理
	std::unique_ptr<WeaponController> weaponController_;

	LWP::Math::Quaternion moveRot_;
	LWP::Math::Quaternion preMoveRot_;
	LWP::Math::Quaternion lockOnOmega_;
	LWP::Math::Quaternion preLockOnOmega_;
	bool isTriggerLockOn_ = false;
};