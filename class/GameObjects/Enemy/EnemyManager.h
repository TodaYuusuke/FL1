#pragma once
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include <Adapter.h>

class IWorld;
/// <summary>
/// 敵管理クラス
/// </summary>
class EnemyManager {
public:
	// コンストラクタ
	EnemyManager(IWorld* world);
	// デストラクタ
	~EnemyManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:
	/// <summary>
	/// 近接敵生成
	/// </summary>
	void CreateMeleeEnemy();
	/// <summary>
	/// テスト敵生成
	/// </summary>
	void CreateTestEnemy();

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 敵リストを取得
	/// </summary>
	/// <returns></returns>
	std::vector<Actor*> GetEnemyList() { return enemies_; }
#pragma endregion

#pragma region Setter

#pragma endregion

private:// 外部から受け取る変数
	IWorld* pWorld_;

private:
	// 敵リスト
	std::vector<Actor*> enemies_;

	// 生成時の識別番号
	int createID_;

	// 敵の生成座標(デバッグ用)
	LWP::Math::Vector3 createPos_;
	// 敵の速度(デバッグ用)
	LWP::Math::Vector3 createVel_;
};

