#pragma once
#include "../../../Componets/BehaviourTree/Actor/BlackBoard.h"
#include <Adapter.h>

class EnemyManager;
class Actor;
/// <summary>
/// 偏差射撃機能
/// </summary>
class LeadingSystem {
public:
	// コンストラクタ
	LeadingSystem(LWP::Object::Camera* camera, BlackBoard* blackBoard);
	// デストラクタ
	~LeadingSystem() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

	void CalFutureTargetPos(float bulletSpeed);

private:
	/// <summary>
	/// 偏差射撃の対象を選択
	/// </summary>
	void SelectLeadingTarget();
	/// <summary>
	/// 偏差射撃の対象解除
	/// </summary>
	void ClearLeadingTarget();

	float PlusMin(float a, float b);

public:// アクセサ
	void Start(LWP::Math::Vector3 shooterPos, float bulletSpeed) {
		shooterPos_ = shooterPos;
		bulletSpeed_ = bulletSpeed;
	}

#pragma region Getter
	/// <summary>
	/// 偏差射撃の角度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetLeadingShotAngle(LWP::Math::Vector3 shooterPos, float bulletSpeed);
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 敵管理クラスのアドレスを設定
	/// </summary>
	/// <param name="enemyManager"></param>
	void SetEnemyManager(EnemyManager* enemyManager) { pEnemyManager_ = enemyManager; }
#pragma endregion

private:// 外部から受け取る変数
	// 敵管理クラス
	EnemyManager* pEnemyManager_;
	// カメラ
	LWP::Object::Camera* pCamera_;

	BlackBoard* blackBoard_;

private:// 調整項目
	// 偏差射撃の対象になる範囲(スクリーン座標基準)
	float leadingScreenRange_ = 700.0f;
	// 偏差射撃の対象になる範囲(ワールド座標基準)
	float leadingWorldRange_ = 100.0f;

	// 偏差射撃の性能
	float leadingAccuracy_ = 1.0f;

private:
	// 偏差対象
	Actor* leadingTarget_;
	LWP::Math::Vector2 leadingTargetScreenPos_;

	// 偏差対象の座標(補間ありの座標)
	LWP::Math::Vector3 leadingTargetPos_;
	// 画面中心のワールド座標
	LWP::Math::Vector3 centerWorldPos_;

	LWP::Math::Vector3 targetFuture_;
	// 算出した角度
	LWP::Math::Quaternion leadingShotAngle_;



	LWP::Math::Vector3 shooterPos_;
	float bulletSpeed_;


	LWP::Primitive::NormalSprite reticle_;
};