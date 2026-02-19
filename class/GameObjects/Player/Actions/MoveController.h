#pragma once
#include "IActions.h"
#include "Action/IAction.h"
#include "Action/Move/Move.h"
#include "Action/Boost/Boost.h"
#include "../../../Componets/BehaviourTree/Actor/BlackBoard.h"
#include "../../../Componets/Input/VirtualController.h"
#include "../../Camera/Effect/CameraEffectHandler.h"
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

private:// 演出制御
	/// <summary>
	/// ブースト時の音再生
	/// </summary>
	void PlayBoostAudio();
	/// <summary>
	/// ブースト時の演出開始
	/// </summary>
	void StartBoostEffect();
	/// <summary>
	/// ブースト時の演出終了
	/// </summary>
	void FinishBoostEffect();
	/// <summary>
	/// ブースト時の演出調整
	/// </summary>
	void AdjustBoostEffect();

public:// アクセサ
#pragma region Getter
	Move* GetMove();
	bool GetIsTurnBehind();
#pragma endregion

#pragma region Setter
	/// <summary>
	/// ループSEの停止関数
	/// </summary>
	void StopAllLoopSE();
#pragma endregion

private:// 調整項目
	float boostCameraFov = 30.0f;
	float boostCameraEffectTime = 2.0f;
	float boostCameraBoundTime = 3.0f / 60.0f;
	LWP::Math::Vector3 boostCameraShake = { 0.02f,0.02f ,0.02f };
	LWP::Math::Vector3 boostCameraBound = { 0.0f,0.1f,0.0f };

private:
	BlackBoard* pBB_;
	VirtualController* vCon_;
	CameraEffectHandler* cameraEffector_;

private:
	// 行動処理をまとめたリスト
	// [0]:ActionType::kMain → Moveクラスしか入れない
	std::map<ActionType, std::unique_ptr<IAction>> actions_;

	// 前フレーム角度
	LWP::Math::Quaternion preRot_ = { 0.0f,0.0f,0.0f,1.0 };

	// 移動時やブースト時のループ効果音のID格納用
	uint32_t moveSEID_{};
	uint32_t boostSEID_{};
	uint32_t quickTurnSEID_{};
};

