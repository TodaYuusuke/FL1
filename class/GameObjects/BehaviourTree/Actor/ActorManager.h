#pragma once
#include <list>
#include <vector>
#include <string>

class Actor;	//アクタークラスの前方宣言

//アクターマネージャー
class ActorManager {
public:
	//デフォルトコンストラクタ
	ActorManager() = default;
	//デストラクタ
	~ActorManager();
	//アクターの追加
	void Add(Actor* actor);
	//アクターの更新
	void Update();
	//アクターのGUI描画
	void DrawGui() const;
	//死亡しているアクターの削除
	void Remove();
	//アクターの検索
	Actor* Find(const std::string& name) const;
	//指定したタグ名を持つアクターの検索
	std::vector<Actor*> FindWithTag(const std::string& tag) const;
	//アクター数を返す
	int Count() const;
	//指定したタグ名を持つアクター数を返す
	int CountWithTag(const std::string& tag) const;
	//消去
	void Clear();
	//コピー禁止
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;

private:
	//アクターリスト
	std::list<Actor*> pActors;
};