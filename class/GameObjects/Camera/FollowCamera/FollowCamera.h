#pragma once
#include <Adapter.h>
#include <numbers>

class Actor;
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
	void SetTarget(Actor* actor) { target_ = actor; }
	/// <summary>
	/// カメラの角度を設定
	/// </summary>
	/// <returns></returns>
	void SetCameraRotate(LWP::Math::Quaternion quat) { camera_->worldTF.rotation = quat; }
#pragma endregion

public:// jsonで保存する値
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
	LWP::Math::Vector3 defaultTargetDist_ = { 0.0f,10.0f,3.0f };

public:// 外部からポインタをもらう変数
	// カメラ
	LWP::Object::Camera* camera_;

private:
	// 追従対象
	Actor* target_;

private:
	// スティックの入力を受け取る
	LWP::Math::Vector3 stickDir_;

	// 後追い座標
	LWP::Math::Vector3 interTarget_;
	// 追従対象との距離
	LWP::Math::Vector3 kTargetDist = { 0.0f,0.0f,1.0f };

	// カメラの補間なし時の座標(自機の移動処理に使うために作成)
	LWP::Object::Camera* debugCamera_;
};

