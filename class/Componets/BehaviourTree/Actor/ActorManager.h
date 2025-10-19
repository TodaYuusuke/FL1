#pragma once
#include <list>
#include <vector>
#include <string>

class Actor;
/// <summary>
/// アクターマネージャー
/// 登録したアクターは自動で解放するので任意で解放するなら登録非推奨
/// </summary>
class ActorManager {
public:
	//デフォルトコンストラクタ
	ActorManager() = default;
	//デストラクタ
	~ActorManager();

	//アクターの更新
	void Update();
	//アクターのGUI描画
	void DrawGui() const;

public:// アクセサ
	/// <summary>
	/// アクターの追加
	/// </summary>
	/// <param name="actor"></param>
	void Add(Actor* actor);
	/// <summary>
	/// 死亡しているアクターの削除
	/// </summary>
	void Remove();
	/// <summary>
	/// 消去
	/// </summary>
	void Clear();

	/// <summary>
	/// アクターの検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	Actor* Find(const std::string& name) const;
	/// <summary>
	/// 指定したタグ名を持つアクターの検索
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	std::vector<Actor*> FindWithTag(const std::string& tag) const;

	/// <summary>
	/// アクター数を返す
	/// </summary>
	/// <returns></returns>
	int Count() const;
	/// <summary>
	/// 指定したタグ名を持つアクター数を返す
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	int CountWithTag(const std::string& tag) const;

public:
	//コピー禁止
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;

private:
	//アクターリスト
	std::list<Actor*> pActors_;
};