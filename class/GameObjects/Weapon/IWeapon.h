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
	/// <summary>
	/// 破壊するかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDestroy() { return isDestroy_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 親子付けの設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(Actor* character) {
		actor_ = character;
		body_.worldTF.Parent(actor_->GetWorldTF());
	}
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="translation"></param>
	void SetTranslation(const LWP::Math::Vector3& translation) { body_.worldTF.translation = translation; }
	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="q"></param>
	void SetRotation(const LWP::Math::Quaternion& q) { body_.worldTF.rotation = q; }
	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="name"></param>
	void SetName(const std::string& name) { name_ = name; }
#pragma endregion

protected:
	// モデル
	LWP::Resource::RigidModel body_;

	Actor* actor_;

	// 名前
	std::string name_;

	// 破壊するか
	bool isDestroy_;
};

