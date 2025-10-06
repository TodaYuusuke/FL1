#pragma once

#include <string>
#include <vector>
#include "LWP.h"
#include "../Math/MyTimer.h"
#include "ActorHealth.h"

class IWorld;	//ワールド抽象インターフェースの前方宣言
class BlackBoard;

//アクタークラス
class Actor {
public:
	//コンストラクタ
	Actor() = default;
	//仮想デストラクタ
	virtual ~Actor() = default;
	//更新
	virtual void Update(float delta_time);
	//描画
	virtual void Draw() const;
	//GUIの描画
	virtual void DrawGui();
	//メッセージ処理
	virtual void HandleMessage(const std::string& message, void* param);
	//死亡する
	void Die();

public:// アクセサ
	/// <summary>
	/// 死亡しているか
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() const;
	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns></returns>
	const std::string& GetName() const;
	/// <summary>
	/// タグ名を取得
	/// </summary>
	/// <returns></returns>
	const std::string& GetTag() const;
	/// <summary>
	/// 現在位置を取得
	/// </summary>
	/// <returns></returns>
	const LWP::Math::Vector3& GetPos() const;

	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	/// <returns></returns>
	float GetAttackPower() const;

	//virtual void damage(const float damage_value);
	
public:
	//コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	//ワールド
	IWorld* world{ nullptr };
	//ブラックボード
	BlackBoard* blackBoard{ nullptr };
	//タグ名
	std::string tag;
	//名前
	std::string name;
	//トランスフォーム
	LWP::Math::Vector3 pos;
	//移動量
	LWP::Math::Vector3 velocity{ 0.0f,0.0f };
	//死亡フラグ
	bool isDead{ false };
	//攻撃力
	float attackPower{ 0.0f };
	// 体力
	ActorHealth health{ 100 };
};