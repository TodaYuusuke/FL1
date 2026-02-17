#pragma once
#include "../IAction.h"
#include "../../../PlayerConfig.h"
#include "../../../../../Componets/Math.h"
#include "../../../../../Componets/BehaviourTree/Actor/BlackBoard.h"
#include "../../../../../Effect/Utility/DeltaTimer.h"

class Move : public IAction {
private:
	// 移動方式
	enum class MoveType {
		kTank,
		kFPS,
		kCount
	};

public:
	// コンストラクタ
	Move(BlackBoard* blackBoard);
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
	/// 180度旋回
	/// </summary>
	void TurnBehind();
	/// <summary>
	/// 加速処理
	/// </summary>
	void BoostUpdate();

	/// <summary>
	/// 戦車のような移動処理
	/// </summary>
	/// <param name="leftStickY"></param>
	/// <param name="rightStickY"></param>
	/// <param name="deltaTime"></param>
	void DifferentialUpdate(LWP::Math::Vector2 leftStick, LWP::Math::Vector2 rightStick, float deltaTime);
	/// <summary>
	/// 一人称視点の移動処理
	/// </summary>
	void FPSTypeMove();
	/// <summary>
	/// 移動方式の確認
	/// </summary>
	void CheckMoveType();

public:
	/// <summary>
	/// コントローラの入力補正
	/// </summary>
	/// <param name="stick"></param>
	/// <returns></returns>
	LWP::Math::Vector2 AdjustmentStick(LWP::Math::Vector2 stick);

	/// <summary>
	/// 移動入力が成立しているかの確認
	/// </summary>
	bool CheckIsMove(LWP::Math::Vector2 leftStick, LWP::Math::Vector2 rightStick);
	bool CheckIsSideMove(float leftStickX, float rightStickX);
	bool CheckIsVerticalMove(float leftStickY, float rightStickY);

private:
	/// <summary>
	/// 移動に伴って自機の体を傾ける
	/// </summary>
	void BodyInclination();

public:
#pragma region Getter
	bool GetIsTurnBehind() { return isTurnBehind_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:
	BlackBoard* pBB_;

private:// 調整項目
	// 入力を検出する範囲
	LWP::Math::Vector2 inputDetectionRange = { 0.8f, 0.01f };

	// 車輪間の距離
	float treadWidth = 2.5f;
	// 移動速度の最高速
	float maxSpeed = 0.8f;
	// 角速度の最高速
	float maxOmega = 0.019f;
	// 旋回の閾値
	float rotThreshold = 0.1f;
	// 180度回転の閾値
	float turnThreshold = 0.85f;
	// 180度旋回にかかる時間
	float turnTime = 0.8f;
	// 体の最大傾き[Degree]
	float maxInclination = 3.14f / 4.0f;

	float maxBoostTime = 0.5f;
	float easeRate = 0.8f;

	LWP::Utility::JsonIO json_;

private:
	// 操作タイプ
	MoveType moveType_;

	LWP::Utility::DeltaTimer easeTimer_;

	float boostSpeed_ = 1.0f;
	float start_ = 1.0f;
	float end_ = 2.0f;

	LWP::Math::Matrix4x4 moveRotMatrix_{ 0.0f,0.0f,0.0f,1.0f };
	LWP::Math::Vector3 preVel_;

	LWP::Math::Vector3 turnRadian_;
	LWP::Math::Vector3 preTurnRadian_;
	// 180度旋回のイージングのタイマー
	LWP::Utility::DeltaTimer turnTime_;

	float angle = 0.0f;
	float omega;
	LWP::Math::Vector2 vL;
	LWP::Math::Vector2 vR;

	bool isMoveTypeChange_;
	bool isPreMoveTypeChange_;
	// 180°回転
	bool isTurnBehind_;
	bool isPreTurnBehind_;

	bool isBound_;
};

