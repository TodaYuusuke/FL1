#pragma once
#include <Adapter.h>

/// <summary>
/// 偏差射撃機能
/// </summary>
class LeadingSystem {
public:
	// コンストラクタ
	LeadingSystem();
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

private:
};