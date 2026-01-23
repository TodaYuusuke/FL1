#pragma once
#include "IWorld.h"
#include "../../Componets/BehaviourTree/Actor/ActorManager.h"
#include <Adapter.h>

/// <summary>
/// ワールドクラス
/// 今回は敵は別のManagerクラスで管理し、ここでは自機や地面等を登録して共通でほしくなるアクターを管理
/// </summary>
class World : public IWorld {
public:
	//コンストラクタ
	World();
	//デストラクタ
	~World();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();
	/// <summary>
	/// 消去
	/// </summary>
	void Clear();

public:
	/// <summary>
	/// 移動エリアの制限
	/// </summary>
	void LimitMoveArea(LWP::Math::Vector3& translation) override;

public:// アクセサ
	/// <summary>
	/// アクターを追加
	/// </summary>
	/// <param name="actor"></param>
	virtual void AddActor(Actor* actor) override;
	/// <summary>
	/// アクターの検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	virtual Actor* FindActor(const std::string& name) const override;
	/// <summary>
	/// 指定したタグ名を持つアクターの検索
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	virtual std::vector<Actor*> FindActorWithTag(const std::string& tag) const override;
	/// <summary>
	/// アクター数を返す
	/// </summary>
	/// <returns></returns>
	virtual int CountActor() const override;
	/// <summary>
	/// 指定したタグ名を持つアクター数を返す
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	virtual int CountActorWithTag(const std::string& tag) const override;

	/// <summary>
	/// 任意のオブジェクトの攻撃力を検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	float FindAttackPower(const std::string& name) override;

public:
	//コピー禁止
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	LWP::Math::Vector3 moveArea = { 300.0f,300.0f,300.0f };

	float moveLimitSpeed_ = 0.1f;

	LWP::Utility::JsonIO json_;

private:
	//アクターマネージャー
	ActorManager actorManager;

	//天球
	LWP::Resource::RigidModel skydome_;

	// 移動禁止平面
	std::map<std::string, LWP::Primitive::NormalSurface> moveLimitSurface_;

	LWP::Math::Vector3 scale_;
};