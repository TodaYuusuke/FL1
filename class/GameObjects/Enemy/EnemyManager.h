#pragma once
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include "EnemyConfig.h"
#include "../Weapon/WeaponConfig.h"
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
	Actor* CreateMeleeEnemy();
	/// <summary>
	/// 遠距離敵生成
	/// </summary>
	Actor* CreateGunnerEnemy();
	/// <summary>
	/// 浮遊敵生成
	/// </summary>
	Actor* CreateDroneEnemy();

	/// <summary>
	/// 武器を付与
	/// </summary>
	/// <param name="actor"></param>
	void GiveWeapon(Actor* actor);

private:// デバッグ用の関数群
	/// <summary>
	/// テスト敵生成(デバッグ用)
	/// </summary>
	Actor* CreateTestEnemy();
	/// <summary>
	/// 選ばれた敵を作成(デバッグ用)
	/// </summary>
	Actor* CreateEnemy();
	/// <summary>
	/// 作成する敵を選択(デバッグ用)
	/// </summary>
	void SelectCreateEnemy();
	/// <summary>
	/// 読み込むjsonファイル選択(デバッグ用)
	/// </summary>
	void SelectJsonFile();
	/// <summary>
	/// デバッグ用変数の情報作成
	/// </summary>
	void CreateDebugData();

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

private:// デバッグ用変数
	// ビヘイビアツリーの編集
	std::unique_ptr<BehaviorTreeGraph> btEditor_;

	// 敵の生成座標(デバッグ用)
	LWP::Math::Vector3 createPos_;
	// 敵の速度(デバッグ用)
	LWP::Math::Vector3 createVel_;

	// 作成する敵の名前
	std::vector<int> createEnemyTypes_;

	// 作成できる敵の名前一覧
	std::vector<std::string> enemyTypePreview_;
	int selectCreateEnemyType_;
	// behaviorTreeのファイル名一覧
	std::vector<std::string> enemyBTFileNamePreview_;
	int selectBTFileName_;

	// 調整された敵のオリジナル(この敵の情報を参考にほかの敵を生成する)
	std::map<int, Actor*> sampleEnemies_;

private:// 外部から受け取る変数
	IWorld* pWorld_;

private:// 調整項目
	float dronefloatHeight_ = 4.0f;

private:
	// 敵リスト
	std::vector<Actor*> enemies_;

	// 生成時の識別番号
	int createID_;
};

