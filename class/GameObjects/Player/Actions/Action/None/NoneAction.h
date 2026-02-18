#pragma once
#include "../IAction.h"

class NoneAction : public IAction {
public:
	// コンストラクタ
	NoneAction(const LWP::Math::Vector3& vel = {0.0f,0.0f,0.0f}, float speed = 0.0f);
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
	// イージングのレート
	float easeRate = 0.01f;

private:
};