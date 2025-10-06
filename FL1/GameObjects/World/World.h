#pragma once

#include "IWorld.h"
#include "../BehaviourTree/Actor/ActorManager.h"

//ワールドクラス
class World : public IWorld {
public:
	//コンストラクタ
	World() = default;
	//デストラクタ
	~World();
	//更新
	void Update(float delta_time);
	//描画
	void Draw() const;
	//消去
	void Clear();

public:// アクセサ
	//アクターを追加
	virtual void AddActor(Actor* actor) override;
	//アクターの検索
	virtual Actor* FindActor(const std::string& name) const override;
	//指定したタグ名を持つアクターの検索
	virtual std::vector<Actor*> FindActorWithTag(const std::string& tag) const override;
	//アクター数を返す
	virtual int CountActor() const override;
	//指定したタグ名を持つアクター数を返す
	virtual int CountActorWithTag(const std::string& tag) const override;
	//メッセージの送信
	virtual void Send_Message(const std::string& message, void* param = nullptr) override;

public:
	//コピー禁止
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	//アクターマネージャー
	ActorManager actorManager;
};