#pragma once
#include <Adapter.h>
#include <numbers>

class ICharacter;
/// <summary>
/// 自機に追従するカメラ
/// </summary>
class FollowCamera {
public:
	// コンストラクタ
	FollowCamera(LWP::Object::Camera* camera);
	// デストラクタ
	~FollowCamera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGUI();

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputHandle();

	/// <summary>
	/// 角度制限の処理
	/// </summary>
	/// <param name="target">制限対象</param>
	/// <param name="distance">二点間の距離</param>
	/// <param name="minLimitAngle">角度の下限値(単位:ラジアン)</param>
	/// <param name="maxLimitAngle">角度の上限値(単位:ラジアン)</param>
	void ClampAngle(float& target, LWP::Math::Vector3 distance, float minLimitAngle, float maxLimitAngle);

public:// アクセサ
	/// <summary>
	/// 追従終了
	/// </summary>
	void ResetTarget(){ target_ = nullptr; }

#pragma region Getter
	/// <summary>
	/// カメラのアドレスを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::Camera* GetCamera() { return camera_; }
	/// <summary>
	/// 線形補間をしていない純粋なカメラ座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetDefaultPos() { return defaultPos_; }
	LWP::Math::Vector3 GetRadian() { return radian_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// Sceneで使用されているmainCameraのアドレスを設定
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(LWP::Object::Camera* camera) { camera_ = camera; }
	/// <summary>
	/// 追従対象の設定
	/// </summary>
	/// <param name="character"></param>
	void SetTarget(ICharacter* character) { target_ = character; }
	/// <summary>
	/// カメラの角度を設定
	/// </summary>
	/// <returns></returns>
	void SetCameraRotate(LWP::Math::Quaternion quat) { camera_->worldTF.rotation = quat; }
	/// <summary>
	/// ラジアンを設定
	/// </summary>
	/// <param name="radian"></param>
	void SetRadian(LWP::Math::Vector3 radian) { radian_ = radian; }
#pragma endregion

public:// jsonで保存する値
	// 追従対象との距離
	LWP::Math::Vector3 kTargetDist = { 0.0f,0.0f,1.0f };
	// 初期角度 
	LWP::Math::Vector3 kStartAngle = { 0.3f, 0.0f, 0.0f };

	// x軸の下限値
	float kMinRotateX = (float)std::numbers::pi / 2.0f - 0.1f;
	// x軸の上限値
	float kMaxRotateX = (float)std::numbers::pi / 2.0f + (float)std::numbers::pi / 4.0f;
	// カメラの感度
	float sensitivity = 1.0f;
	// カメラの補間レート
	float interTargetRate = 0.5f;

	float maxLength = 50.0f;

	// 追従対象との距離を補間するレート
	float targetDistRate = 0.1f;
	// カメラの回転補間レート
	float rotateRate = 0.1f;

	// 回転角の始点
	const float kOriginRotateX = 90.0f;

	// 追従対象との初期距離
	LWP::Math::Vector3 defaultTargetDist_;

public:// 外部からポインタをもらう変数
	// カメラ
	LWP::Object::Camera* camera_;

private:
	// 追従対象
	ICharacter* target_;

private:
	// スティックの入力を受け取る
	LWP::Math::Vector3 stickDir_;

	// 後追い座標
	LWP::Math::Vector3 interTarget_;
	// カメラの補間なし時の座標(自機の移動処理に使うために作成)
	LWP::Math::Vector3 defaultPos_;
	// 角度[ラジアン]
	LWP::Math::Vector3 radian_;
};

