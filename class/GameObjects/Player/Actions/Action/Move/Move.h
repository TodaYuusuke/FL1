#pragma once
#include "../IAction.h"
#include "../../../../../Componets/Math.h"

class Move : public IAction {
public:
	// コンストラクタ
	Move();
	// デストラクタ
	~Move() override = default;

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
	/// コントローラの入力補正
	/// </summary>
	/// <param name="stick"></param>
	/// <returns></returns>
	LWP::Math::Vector2 AdjustmentStick(LWP::Math::Vector2 stick);

	/// <summary>
	/// 戦車のような移動処理
	/// </summary>
	/// <param name="leftStickY"></param>
	/// <param name="rightStickY"></param>
	/// <param name="deltaTime"></param>
	void DifferentialUpdate(LWP::Math::Vector2 leftStick, LWP::Math::Vector2 rightStick, float deltaTime);

public:
#pragma region Getter

#pragma endregion

#pragma region Setter

#pragma endregion

private:// 調整項目
	// 車輪間の距離
	float treadWidth = 2.5f;
	// 移動速度の最高速
	float maxSpeed = 0.3f;
	// 角速度の最高速
	float maxOmega = 0.3f;

private:
	float angle = 0.0f;
	float omega;
	float vL = 0.0f;
	float vR = 0.0f;
};

