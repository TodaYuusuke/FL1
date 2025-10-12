#pragma once
#include "WeaponConfig.h"
#include "../../Componets/Math.h"
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include <Adapter.h>
#include <string>

/// <summary>
/// 武器クラスのインターフェイス
/// </summary>
class IWeapon {
public:
	// コンストラクタ
	IWeapon() = default;
	// デストラクタ
	virtual ~IWeapon() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 調整項目
	/// </summary>
	virtual void DebugGui() = 0;

	/// <summary>
	/// 攻撃
	/// </summary>
	virtual void Attack() = 0;

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns></returns>
	std::string GetName() { return name_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="name"></param>
	void SetName(const std::string& name) { name_ = name; }
	/// <summary>
	/// 親子付けの設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(Actor* character) {
		actor_ = character;
		body_.worldTF.Parent(actor_->GetWorldTF());
	}
#pragma endregion

protected:
	// モデル
	LWP::Resource::RigidModel body_;

	Actor* actor_;

	// 名前
	std::string name_;
};

