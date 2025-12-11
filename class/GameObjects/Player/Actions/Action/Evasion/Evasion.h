#pragma once
#include "../IAction.h"

class Evasion : public IAction {
public:
	// コンストラクタ
	Evasion(const LWP::Math::Vector3& dirVec, const LWP::Math::Vector3& startPos);
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

private:// 調整項目
	// 
	float maxEvasionFrame = 20.0f;

	float maxEvasionDist = 50.0f;

private:
	LWP::Math::Vector3 startPos_;
	LWP::Math::Vector3 endPos_;
};

