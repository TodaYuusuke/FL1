#pragma once
#include "../../Componets/Math.h"
#include "../../Componets/HitStopController.h"
#include "AttackConfig.h"
#include <Adapter.h>

class Actor;
class AttackBase {
public:
	// コンストラクタ
	AttackBase(int hitFragBit);
	// デストラクタ
	virtual ~AttackBase() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() {}
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {}

protected:
	/// <summary>
	/// 衝突応答
	/// </summary>
	virtual void OnCollision(LWP::Object::Collision* hitTarget) = 0;

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::TransformQuat* GetWorldTF() { return &body_.worldTF; }
	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return vel_; }
	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns></returns>
	std::string GetName() { return bodyCollision_.name; }
	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	/// <returns></returns>
	float GetAttackPower() { return attackPower_; }
	/// <summary>
	/// 攻撃倍率を取得
	/// </summary>
	/// <returns></returns>
	float GetAttackMultiply() { return attackMultiply_; }
	/// <summary>
	/// 速度倍率を取得
	/// </summary>
	/// <returns></returns>
	float GetSpeedMultiply() { return speedMultiply_; }
	/// <summary>
	/// 当たり判定をとる対象のマスクを設定
	/// </summary>
	/// <returns></returns>
	int GetHitFragBit() { return hitFragBit_; }
	/// <summary>
	/// 生存しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAlive() { return isAlive_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 攻撃対象を設定
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(Actor* target) { target_ = target; }
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const LWP::Math::Vector3& pos) { body_.worldTF.translation = pos; }
	/// <summary>
	/// 角度を設定
	/// </summary>
	/// <param name="q"></param>
	void SetRotation(const LWP::Math::Quaternion& q) { body_.worldTF.rotation = q; }
	/// <summary>
	/// 速度ベクトルを設定
	/// </summary>
	/// <param name="vel"></param>
	void SetVelocity(const LWP::Math::Vector3& vel) { vel_ = vel; }
	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="name"></param>
	void SetName(const std::string& name) { bodyCollision_.name = name; }
	/// <summary>
	/// 攻撃力を設定
	/// </summary>
	/// <returns></returns>
	void SetAttackPower(float value) { attackPower_ = value; }
	/// <summary>
	/// 攻撃倍率を設定
	/// </summary>
	/// <param name="value"></param>
	void SetAttackMultiply(float value) { attackMultiply_ = value; }
	/// <summary>
	/// 速度倍率を設定
	/// </summary>
	/// <param name="value"></param>
	void SetSpeedMultiply(float value) { speedMultiply_ = value; }
#pragma endregion

protected:// 外部から受け取る変数
	// ヒットストップ
	HitStopController* stopController_;
	// 攻撃対象(特定の処理でのみ使う)
	Actor* target_;

protected:
	// モデル
	LWP::Resource::RigidModel body_;
	// 体の当たり判定
	LWP::Object::Collision bodyCollision_;

	// 速度
	LWP::Math::Vector3 vel_ = { 0.0f,0.0f,1.0f };

	// 攻撃力
	float attackPower_;
	// 攻撃倍率
	float attackMultiply_ = 1.0f;
	// 移動速度
	float moveSpeed_;
	// 速度倍率
	float speedMultiply_ = 1.0f;

	float currentFrame_ = 60.0f;

	// 当たり判定をとる対象のマスクを設定
	int hitFragBit_;

	// 生存しているか
	bool isAlive_ = true;
};

