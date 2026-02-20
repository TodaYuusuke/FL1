#pragma once
#include <string>
#include <vector>
#include <Adapter.h>
#include "Health.h"
#include "../BehaviorTreeUtility.h"
#include "../INode.h"
#include "../../HitStopController.h"
#include "../../../GameObjects/Enemy/EnemyConfig.h"
#include "../../../Audio/AudioPlayer.h"

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
	Actor();
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
	/// json情報作成(コピー元武器作成時以外呼び出し禁止)
	/// </summary>
	virtual void CreateJsonData();

	/// <summary>
	/// 攻撃する
	/// </summary>
	virtual void Attack();

public: // アニメーション用関数群

	/// <summary>
	/// アニメーションマネージャーの更新関数
	/// </summary>
	virtual void AnimManagerUpdate() {};

	/// <summary>
	/// 射撃アニメーション再生関数
	/// </summary>
	/// <param name="weaponSide">武器の場所</param>
	virtual void PlayAttackAnim(const int weaponSide = 0) { weaponSide; };

	/// <summary>
	/// 取得アニメーション再生関数
	/// </summary>
	/// <param name="weaponSide">取得した部位</param>
	/// <param name="data">取得した武器のデータ</param>
	virtual void PlayPickUpAnim(const int weaponSide = 0, const WeaponData& data = {}) { weaponSide; data; }

	/// <summary>
	/// 押し出し時呼び出される処理
	/// </summary>
	/// <param name="targetName">衝突対象名称</param>
	virtual void ResolvedCollision(const std::string& targetName) { targetName; };

protected:
	/// <summary>
	/// 衝突応答
	/// </summary>
	virtual void OnCollision(LWP::Object::Collision* hitTarget);

public:// アクセサ
	/// <summary>
	/// 状態の変更
	/// </summary>
	void ChangeState(StateBase* nextState);

	/// <summary>
	/// ダメージ開始
	/// </summary>
	/// <param name="value">減少量</param>
	void Damage(float value) { hp_->Damage(value); }

#pragma region Getter
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
	/// 調整情報を取得
	/// </summary>
	/// <returns></returns>
	EnemyData GetEnemyData() { return data_; }
	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns></returns>
	Health* GetHP() { return hp_.get(); }
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
	/// 所属当たり判定を取得
	/// </summary>
	/// <returns></returns>
	int GetBelongFrag() { return bodyCollision_.mask.GetBelongFrag(); }
	/// <summary>
	/// 持っている武器を取得
	/// </summary>
	/// <returns></returns>
	std::map<int, IWeapon*> GetWeapon() { return weapons_; }
	IWeapon* GetWeapon(int weaponSide) { return weapons_[weaponSide]; }
	/// <summary>
	/// 現在位置を取得
	/// </summary>
	/// <returns></returns>
	LWP::Resource::SkinningModel* GetModel() { return &model_; }
	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::TransformQuat* GetWorldTF() { return &model_.worldTF; }
	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3* GetVelocity() { return &velocity_; }
	/// <summary>
	/// 武器由来の速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetWeaponVelocity() { return weaponVel_; }
	/// <summary>
	/// 角度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetRot() { return quat_; }
	/// <summary>
	/// 演出用の角度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Quaternion GetEffectRot() { return effectRot_; }
	/// <summary>
	/// 中心座標を取得
	/// </summary>
	/// <returns></returns>
	virtual LWP::Math::Vector3 GetCenterPosition() { return model_.worldTF.GetWorldPosition(); }
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
	/// <summary>
	/// 生きているか取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAlive() { return isAlive_; }
	/// <summary>
	/// エリア外にいるかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsLimitMoveArea() { return isLimitMoveArea_; }
	/// <summary>
	/// 撃墜スコア取得
	/// </summary>
	/// <returns></returns>
	int32_t GetScore() { return score_; };
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 持っている武器を取得
	/// </summary>
	/// <returns></returns>
	virtual void SetWeapon(IWeapon* weapon, int weaponSide);
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
	/// 武器由来の速度を設定
	/// </summary>
	/// <param name="velocity"></param>
	void SetWeaponVelocity(const LWP::Math::Vector3& velocity) { weaponVel_ = velocity; }
	/// <summary>
	/// 角度を設定
	/// </summary>
	/// <param name="q"></param>
	void SetRot(const LWP::Math::Quaternion& q) { quat_ = q; }
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
	/// <summary>
	/// 所属当たり判定を設定
	/// </summary>
	/// <param name="belongFrag"></param>
	void SetBelongFrag(int belongFrag) { bodyCollision_.mask.SetBelongFrag(belongFrag); }
	/// <summary>
	/// 生きているか設定
	/// </summary>
	/// <returns></returns>
	void SetIsAlive(const bool& isAlive) { isAlive_ = isAlive; }
#pragma endregion

public:
	//コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	// ヒットストップ
	HitStopController* stopController_;

	//ワールド
	IWorld* world_ = nullptr;

	//ブラックボード
	BlackBoard* blackBoard_ = nullptr;
	// ビヘイビアツリー
	INode* bt_ = nullptr;
	// ビヘイビアツリーの編集
	std::unique_ptr<BehaviorTreeGraph> btEditor_;

	// 状態
	StateBase* state_;
	// 持っている武器リスト
	std::map<int, IWeapon*> weapons_;

	// 体の当たり判定
	LWP::Object::Collision bodyCollision_;
	LWP::Object::Collider::AABB& bodyAABB_;

	EnemyData data_;

	//タグ名
	std::string tag_;
	//名前
	std::string name_;

	// モデル
	LWP::Resource::SkinningModel model_;
	std::vector<std::string> jointName_;
	// 角度
	LWP::Math::Quaternion quat_{ 0.0f,0.0f,0.0f,1.0f };
	// 演出用の角度
	LWP::Math::Quaternion effectRot_ = { 0.0f,0.0f,0.0f,1.0 };
	// 移動量
	LWP::Math::Vector3 velocity_{ 0.0f,0.0f,0.0f };
	// 武器由来の速度
	LWP::Math::Vector3 weaponVel_{ 0.0f,0.0f,0.0f };
	// 前回の座標
	LWP::Math::Vector3 preTranslation_ = { 0.0f,0.0f,0.0f };

	// 体力
	std::unique_ptr<Health> hp_;
	// 攻撃力
	float attackPower_ = 0.0f;

	// 生存フラグ
	bool isAlive_ = true;
	// エリア外にいるか
	bool isLimitMoveArea_ = false;

	//スコア(仮値全オブジェクト100)
	int32_t score_ = 12345;

	// 死亡時効果音のID
	uint32_t deadSEID_{};
};