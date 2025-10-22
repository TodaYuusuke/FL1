#pragma once
#include "../../../Componets/BehaviourTree/Actor/ActorHealth.h"
#include "../../../Componets/BehaviourTree/BehaviorTreeUtility.h"
#include "../../../Componets/BehaviourTree/Actor/Actor.h"
#include "../../../Componets/BehaviourTree/INode.h"
#include <Adapter.h>

class IWorld;
class Drone : public Actor {
public:
	//コンストラクタ
	Drone(IWorld* world, int ID, const std::string& BTFilePath);
	//デストラクタ
	~Drone() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 調整項目
	/// </summary>
	void DrawGui() override;

public:
#pragma region Getter
	// BehaviorTreeの中で現在動いているノードのIDを取得
	const int GetBTRunningNodeID() const;
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 浮遊する高さを設定
	/// </summary>
	/// <param name="height"></param>
	void SetFloatHeight(float height) { model_.worldTF.translation.y = height; }
#pragma endregion

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

private:// 調整項目

private:
	// 識別番号
	int ID_;
};
