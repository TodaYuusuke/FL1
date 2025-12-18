#pragma once
#include "../CameraEffect.h"
#include "../../../../Componets/Shake.h"
#include "../../../../Effect/Utility/DeltaTimer.h"

class FollowCamera;
/// <summary>
/// カメラを揺らす
/// </summary>
class CameraBound : public CameraEffect {
public:
	// コンストラクタ
	CameraBound(FollowCamera* camera, LWP::Math::Vector3 moveValue, float endTime);
	// デストラクタ
	~CameraBound() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 開始
	/// </summary>
	void Start(LWP::Math::Vector3 moveValue, float endTime);
	/// <summary>
	/// 強制終了
	/// </summary>
	void Finish();

private:
	/// <summary>
	/// 跳ねる挙動
	/// </summary>
	void BoundMove();

private:
	LWP::Math::Vector3 boundValue_;
	LWP::Math::Vector3 endBoundValue_;

	float currentFrame_;
	float endTime_;
};

