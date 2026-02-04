#pragma once
#include "System/LeadingSystem.h" 
#include "Actions/MoveController.h"
#include "Actions/WeaponController.h"
#include "System/WeaponSkill/WeaponSkill.h"
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include "../Camera/FollowCamera/FollowCamera.h"
#include "../UI/SingleGauge/SingleGauge.h"
#include "PlayerConfig.h"
#include <Adapter.h>
#include "Anim/RobotAnimManager.h"

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
	/// <summary>
	/// 衝突応答
	/// </summary>
	void OnCollision(LWP::Object::Collision* hitTarget) override;

public: // アニメーション用関数群

	/// <summary>
	/// 射撃アニメーション再生関数
	/// </summary>
	/// <param name="weaponSide">武器の場所</param>
	void PlayAttackAnim(const int weaponSide = 0) override;

	/// <summary>
	/// 取得アニメーション再生関数
	/// </summary>
	/// <param name="weaponSide">取得した部位</param>
	void PlayPickUpAnim(const int weaponSide = 0) override;

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
	/// ComboGuiの表示
	/// </summary>
	/// <param name="list"></param>
	/// <param name="selectedType"></param>
	/// <param name="label"></param>
	void SelectType(std::vector<std::string> list, int& selectedType, std::string label, bool& isClickCombo);
	void SelectType(std::vector<std::string> list, int& selectedType, std::string label);
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

	bool Contains(const std::string& str, const std::string& target) {
		return str.find(target) != std::string::npos;
	}

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 移動管理クラスを取得
	/// </summary>
	/// <returns></returns>
	MoveController* GetMoveController() { return moveController_.get(); }
	/// <summary>
	/// 武器管理クラスを取得
	/// </summary>
	/// <returns></returns>
	WeaponController* GetWeaponController() { return weaponController_.get(); }
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
	std::array<int, (int)WeaponSide::kCount> weaponNamesIndex_;

private:
	float maxHp_ = 100.0f;
	float maxSpeed_ = 1.0f;

private: //HP表示関係の調整項目
	LWP::Math::Vector3 hpGaugePosition_;
	LWP::Math::Vector3 hpGaugeScale_;
	float hpGaugeRotate_;

private:
	LWP::Utility::JsonIO json_;

	// 偏差射撃機能
	std::unique_ptr<LeadingSystem> leadingSystem_;

	// 移動行動系の処理
	std::unique_ptr<MoveController> moveController_;
	// 武器の処理
	std::unique_ptr<WeaponController> weaponController_;

	// アニメーションマネージャー
	std::unique_ptr<RobotAnimManager> animManager_;

	LWP::Math::Quaternion moveRot_;
	LWP::Math::Quaternion preMoveRot_;
	LWP::Math::Quaternion lockOnOmega_;
	LWP::Math::Quaternion preLockOnOmega_;
	bool isTriggerLockOn_ = false;

	//UI
	std::unique_ptr<SingleGauge> hpGauge_;
};