#pragma once
#include "../IAction.h"

class Evasion : public IAction {
public:
	// コンストラクタ
	Evasion();
	// デストラクタ
	~Evasion() override = default;

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

