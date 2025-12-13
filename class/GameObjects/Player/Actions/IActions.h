#pragma once
#include "Action/IAction.h"
#include "../../../Componets/Math.h"
#include <memory>
#include <map>

class IActions {
public:
	// コンストラクタ
	IActions() = default;
	// デストラクタ
	virtual ~IActions() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 調整項目
	/// </summary>
	virtual void DebugGui() = 0;

protected:
	/// <summary>
	/// 状態の変更
	/// </summary>
	/// <param name="currentAction"></param>
	/// <param name="nextAction"></param>
	void ChangeState(std::unique_ptr<IAction>& currentAction, std::unique_ptr<IAction> nextAction) {
		// 同じ方なら終了
		if (currentAction && typeid(*nextAction) == typeid(*currentAction)) {
			nextAction.reset();
			return;
		}
		// 状態が生成されているときだけ解放
		if (currentAction) currentAction = std::move(nextAction);
	}
	/// <summary>
	/// 変更可能な状態かを取得
	/// </summary>
	/// <returns></returns>
	bool CheckEnableChangeState(const unsigned int& targetStateMask, const unsigned int& enableChangeStateMask) {
		if (targetStateMask & enableChangeStateMask) return true;
		return false;
	}
	/// <summary>
	/// 並列可能な状態同士であるかを取得
	/// </summary>
	/// <param name="stateMask"></param>
	/// <returns></returns>
	bool CheckInabilityParallelState(const unsigned int& targetStateMask, const unsigned int& inabilityParallelStateMask) {
		// 並列禁止マスクがある場合はfalse
		if (targetStateMask & inabilityParallelStateMask) return false;
		return true;
	}

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 変更可能な状態マスクを取得
	/// </summary>
	/// <returns></returns>
	unsigned int GetEnableChangeState() { return enableChangeState_; }
	/// <summary>
	/// 並列不能な状態マスクを取得
	/// </summary>
	/// <returns></returns>
	unsigned int GetInabilityParallelState() { return inabilityParallelState_; }
	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVel() { return vel_; }
	/// <summary>
	/// 角度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetRot() { return rot_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 角度を設定
	/// </summary>
	/// <param name="rot"></param>
	void SetRot(const LWP::Math::Quaternion& rot) { rot_ = rot; }
#pragma endregion

protected:
	// 変更可能な状態マスク
	unsigned int enableChangeState_ = 0;
	// 並列不能な状態マスク
	unsigned int inabilityParallelState_ = 0;

	// 移動速度
	LWP::Math::Vector3 vel_;
	// 角度
	LWP::Math::Quaternion rot_ = { 0.0f,0.0f,0.0f,1.0 };
};