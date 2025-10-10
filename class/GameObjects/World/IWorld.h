#ifndef IWORLD_H_
#define IWORLD_H_

#include <string>
#include <vector>

class Actor;

//ワールド抽象インターフェース
class IWorld {
public:
	//仮想デストラクタ
	virtual ~IWorld() = default;

	//アクターを追加
	virtual void AddActor(Actor* actor) = 0;
	//アクターの検索
	virtual Actor* FindActor(const std::string& name) const = 0;
	//指定したタグ名を持つアクターの検索
	virtual std::vector<Actor*> FindActorWithTag(const std::string& tag) const = 0;
	//アクター数を返す
	virtual int CountActor() const = 0;
	//指定したタグ名を持つアクター数を返す
	virtual int CountActorWithTag(const std::string& tag) const = 0;
};

#endif // !IWORLD_H_
