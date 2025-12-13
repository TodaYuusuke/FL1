#pragma once
#include "IActions.h"
#include "Action/IAction.h"
#include "Action/Boost/Boost.h"
#include "../../../Componets/BehaviourTree/Actor/BlackBoard.h"
#include <memory>
#include <vector>

class Actor;
/// <summary>
/// 自機の移動に関する機能クラス
/// </summary>
class MoveController : public IActions {
public:
	// コンストラクタ
	MoveController(BlackBoard* blackBoard);
	// デストラクタ
	~MoveController() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui() override;

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputHandle();

public:// アクセサ
#pragma region Getter
	
#pragma endregion

#pragma region Setter

#pragma endregion

private:
	BlackBoard* pBB_;

private:
	// 行動処理をまとめたリスト
	std::map<ActionType, std::unique_ptr<IAction>> actions_;
};

