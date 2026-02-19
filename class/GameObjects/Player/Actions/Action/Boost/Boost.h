#pragma once
#include "../IAction.h"
#include "../../../../../Effect/Utility/DeltaTimer.h"

class Boost : public IAction {
public:
	// コンストラクタ
	Boost(const LWP::Math::Vector3& dirVec, const LWP::Math::Vector3& startPos);
	// デストラクタ
	~Boost() override = default;

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

public:
	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVel() override { return vel_; }

	/// <summary>
	/// 方向ベクトルの設定
	/// </summary>
	/// <param name="vel"></param>
	void SetVel(const LWP::Math::Vector3& vel) override { boostDir_ = vel; }

private:// 調整項目
	float maxBoostTime = 0.5f;
	float easeRate = 0.1f;
	LWP::Utility::JsonIO json_;

private:
	LWP::Utility::DeltaTimer easeTimer_;

	LWP::Math::Vector3 boostDir_;

	float start_ = 1.0f;
	float end_ = 2.0f;
};

