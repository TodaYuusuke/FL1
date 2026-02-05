#pragma once
#include "IActions.h"
#include "Action/IAction.h"
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

public:// アクセサ
#pragma region Getter
	bool GetIsTurnBehind();
#pragma endregion

#pragma region Setter

#pragma endregion

private:// 調整項目
	float boostCameraFov = 15.0f;
	float boostCameraEffectTime = 0.3f;
	float boostCameraBoundTime = 3.0f / 60.0f;
	LWP::Math::Vector3 boostCameraShake = { 0.02f,0.02f ,0.02f };
	LWP::Math::Vector3 boostCameraBound = { 0.0f,0.1f,0.0f };

private:
	BlackBoard* pBB_;
	VirtualController* vCon_;
	CameraEffectHandler* cameraEffector_;

private:
	// 行動処理をまとめたリスト
	std::map<ActionType, std::unique_ptr<IAction>> actions_;

	// 移動時やブースト時のループ効果音のID格納用
	uint32_t moveSEID_{};
	uint32_t boostSEID_{};
};

