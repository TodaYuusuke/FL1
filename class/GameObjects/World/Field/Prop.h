#pragma once
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "PropStructs.h"

/// <summary>
/// フィールド配置物
/// </summary>
class Prop : public Actor
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	Prop() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name"></param>
	/// <param name="filePath">モデルまでのファイルパス</param>
	/// <param name="radius">コライダー半径</param>
	/// <param name="priority">判定優先度</param>
	Prop(const std::string& name, const std::string& filePath, const float radius = 1.0f, const int priority = 2);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data">読み込み済みデータ</param>
	Prop(const LWP::Prop::PropSaveData& data);

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	~Prop() = default;

public: // メンバ関数

	/// <summary>
	/// デバッグGUI
	/// </summary>
	void DrawGui() override;

	/// <summary>
	/// (ユーザー呼び出し禁止) ImGui上に対応するラジオボタンを表示させる関数
	/// </summary>
	/// <param name="id">ラジオボタン判定用ID</param>
	/// <param name="buttonID">ボタンID</param>
	/// <param name="targetProp">ImGuiに表示する接地物</param>
	void ImGuiRadioButton(int& id, int& buttonID, Prop*& targetProp);

public: // アクセッサ等

	/// <summary>
	/// 配置物の編集情報を受け取る
	/// </summary>
	/// <returns>保存用データ</returns>
	LWP::Prop::PropSaveData* GetData();

	/// <summary>
	/// 優先度取得
	/// </summary>
	/// <returns>優先度</returns>
	int GetPriority() { return priority_; }

	/// <summary>
	/// コライダー半径取得
	/// </summary>
	/// <returns>半径</returns>
	float GetRadius() { return radius_; }

private: // プライベートなメンバ関数

	/// <summary>
	/// 衝突応答
	/// </summary>
	void OnCollision(LWP::Object::Collision* hitTarget) override;

private: // 継承先メンバ変数

	// 当たり判定優先度
	int priority_= 2;
	// 当たり判定半径
	float radius_ = 1.0f;

	// 弾用コライダー
	LWP::Object::Collider::Capsule& bodyCapsule_;
	// 弾用コライダー半径
	float capsuleRadius_ = 1.0f;
	// 弾用コライダー高さ
	float capsuleHeight_ = 1.0f;

#pragma region デバッグ用変数

	// コライダー用球
	LWP::Primitive::Sphere colliderSphere_;

	// 保存用データ
	LWP::Prop::PropSaveData data_{};

	// GUI上で選択されているか
	bool isSelected_ = false;

#pragma endregion


};
