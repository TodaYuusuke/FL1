#pragma once
#include "../../../Componets/BehaviourTree/Actor/BlackBoard.h"
#include "../../../Componets/BehaviourTree/NodeResult.h"
#include "../../../Componets/Math.h"
#include "../EnemyConfig.h"
#include <Adapter.h>

/// <summary>
/// 状態の基底クラス
/// </summary>
class StateBase {
public:
	// コンストラクタ
	StateBase() = default;
	// デストラクタ
	virtual ~StateBase() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	///	更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 調整項目
	/// </summary>
	virtual void DebugGui() = 0;

public:
	/// <summary>
	/// 状態の変更
	/// </summary>
	/// <param name="state"></param>
	void ChangeState(StateBase* currentState, StateBase* nextState);

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVel() { return velocity_; }
	/// <summary>
	/// 角度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetRot() { return quat_; }
	/// <summary>
	/// 状態変更ができるかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsEnableChangeState() { return isEnableChangeState_; }
#pragma endregion

#pragma region Setter

#pragma endregion

protected:
	// 黒板(ビヘイビアツリーにて使用)
	BlackBoard* pBlackBoard_;
	NodeResult* nodeResult_;

	// 速度
	LWP::Math::Vector3 velocity_;
	// 角度
	LWP::Math::Quaternion quat_;

	// 状態変更できるか
	bool isEnableChangeState_ = false;
};