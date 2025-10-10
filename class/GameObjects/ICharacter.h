#pragma once
#include <Adapter.h>
#include "BehaviourTree/Actor/Actor.h"
#include "../Componets/Math.h"

/// <summary>
/// 出現する敵や自機の基底クラス
/// </summary>
class ICharacter : public Actor {
public:
	// コンストラクタ
	ICharacter() = default;
	// デストラクタ
	virtual ~ICharacter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override = 0;
	/// <summary>
	/// 調整項目
	/// </summary>
	virtual void DrawGui() override = 0;

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <returns></returns>
	LWP::Resource::RigidModel GetModel() { return model_; }
	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	LWP::Object::TransformQuat* GetWorldTF() { return &model_.worldTF; }
#pragma endregion

#pragma region Setter

#pragma endregion

protected:
	// モデル
	LWP::Resource::RigidModel model_;
};