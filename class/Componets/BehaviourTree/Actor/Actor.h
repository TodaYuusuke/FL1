#pragma once
#include <string>
#include <vector>
#include <Adapter.h>
#include "ActorHealth.h"
#include "../BehaviorTreeUtility.h"
#include "../INode.h"

class IWorld;
class BlackBoard;
class StateBase;
class IWeapon;

/// <summary>
/// アクタークラス
/// </summary>
class Actor {
public:
	// コンストラクタ
	Actor() = default;
	// デストラクタ
	virtual ~Actor() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	virtual void DrawGui();

	/// <summary>
	/// 攻撃する
	/// </summary>
	virtual void Attack();
	/// <summary>
	/// 死亡する
	/// </summary>
	void Die();

public:// アクセサ
	/// <summary>
	/// 状態の変更
	/// </summary>
	void ChangeState(StateBase* nextState);
	/// <summary>
	/// 武器の変更
	/// </summary>
	void ChangeWeapon(IWeapon* nextWeapon);

#pragma region Getter
	/// <summary>
	/// 生きているか取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAlive() { return isAlive_; };
	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns></returns>
	std::string GetName() { return name_; }
	/// <summary>
	/// タグ名を取得
	/// </summary>
	/// <returns></returns>
	std::string GetTag() { return tag_; }
	/// <summary>
	/// ビヘイビアツリーの取得
	/// </summary>
	/// <returns></returns>
	INode* GetBhaviorTree() { return bt_; }
	/// <summary>
	/// ノード編集のキャンバス取得
	/// </summary>
	/// <returns></returns>
	ImNodesEditorContext* GetEditorContext() { return btEditor_->GetEditorContext(); }
	/// <summary>
	/// 状態を取得
	/// </summary>
	/// <returns></returns>
	StateBase* GetState() { return state_; }
	/// <summary>
	/// 持っている武器を取得
	/// </summary>
	/// <returns></returns>
	IWeapon* GetWeapon() { return weapon_; }
	/// <summary>
	/// 現在位置を取得
	/// </summary>
	/// <returns></returns>
	LWP::Resource::RigidModel GetModel() { return model_; }
	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::TransformQuat* GetWorldTF() { return &model_.worldTF; }
	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 前回の座標を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetPreTranslation() { return preTranslation_; }
	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	/// <returns></returns>
	float GetAttackPower() { return attackPower_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 生きているか設定
	/// </summary>
	/// <returns></returns>
	void SetIsAlive(const bool& isAlive) { isAlive_ = isAlive; }
	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <returns></returns>
	void SetName(const std::string& name) { name_ = name; }
	/// <summary>
	/// タグ名を設定
	/// </summary>
	/// <returns></returns>
	void SetTag(const std::string& tag) { tag_ = tag; }
	/// <summary>
	/// 速度を設定
	/// </summary>
	/// <param name="velocity"></param>
	void SetVelocity(const LWP::Math::Vector3& velocity) { velocity_ = velocity; }
	/// <summary>
	/// 親を設定
	/// </summary>
	/// <returns></returns>
	void SetParent(LWP::Object::TransformQuat* parent) { model_.worldTF.Parent(parent); }
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="translation"></param>
	void SetTranslation(const LWP::Math::Vector3& translation) { model_.worldTF.translation = translation; }
	/// <summary>
	/// 前回の座標を設定
	/// </summary>
	/// <param name="translation"></param>
	void SetPreTranslation(const LWP::Math::Vector3& translation) { preTranslation_ = translation; }
	/// <summary>
	/// 大きさを設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const LWP::Math::Vector3& scale) { model_.worldTF.scale = scale; }
	/// <summary>
	/// 角度を設定
	/// </summary>
	/// <param name="q"></param>
	void SetRotation(const LWP::Math::Quaternion& q) { model_.worldTF.rotation = q; }
	/// <summary>
	/// 攻撃力を設定
	/// </summary>
	/// <returns></returns>
	void SetAttackPower(const float& attackPower) { attackPower_ = attackPower; }
#pragma endregion

public:
	//コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	//ワールド
	IWorld* world_ = nullptr;
	//ブラックボード
	BlackBoard* blackBoard_ = nullptr;
	// 状態
	StateBase* state_;
	// 持っている武器
	IWeapon* weapon_;

	// ビヘイビアツリー
	INode* bt_ = nullptr;
	// ビヘイビアツリーの編集
	std::unique_ptr<BehaviorTreeGraph> btEditor_;

	//タグ名
	std::string tag_;
	//名前
	std::string name_;

	// モデル
	LWP::Resource::RigidModel model_;
	// 移動量
	LWP::Math::Vector3 velocity_{ 0.0f,0.0f };
	// 前回の座標
	LWP::Math::Vector3 preTranslation_ = { 0.0f,0.0f,0.0f };

	// 体力
	ActorHealth health_{ 100 };
	//攻撃力
	float attackPower_ = 0.0f;

	// 生存フラグ
	bool isAlive_ = true;
};