#pragma once
#include <Adapter.h>

class FollowCamera;
class CameraEffect {
public:
	// コンストラクタ
	CameraEffect(FollowCamera* camera);
	// デストラクタ
	virtual ~CameraEffect() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

public:// アクセサ
	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return vel_; }

protected:// 外部から受け取る変数
	FollowCamera* pCamera_;

protected:
	// 速度
	LWP::Math::Vector3 vel_;
};

