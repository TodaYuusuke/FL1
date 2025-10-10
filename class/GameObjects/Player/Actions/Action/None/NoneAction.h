#pragma once
#include "../IAction.h"

class NoneAction : public IAction {
public:
	// コンストラクタ
	NoneAction();
	// デストラクタ
	~NoneAction() override = default;

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
};