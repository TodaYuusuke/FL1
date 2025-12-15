#pragma once
#include "../CameraEffect.h"
#include "../../../../Componets/Shake.h"
#include "../../../../Effect/Utility/DeltaTimer.h"

class FollowCamera;
/// <summary>
/// カメラを揺らす
/// </summary>
class CameraZoom : public CameraEffect {
public:
	// コンストラクタ
	CameraZoom(FollowCamera* camera, float zoomValue, float endTime);
	// デストラクタ
	~CameraZoom() override = default;

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
	void Start(float zoomValue, float endTime);
	/// <summary>
	/// 強制終了
	/// </summary>
	void Finish();

private:
	std::unique_ptr<LWP::Utility::DeltaTimer> timer_;

	float zoomValue_;
	float endZoom_;
};

