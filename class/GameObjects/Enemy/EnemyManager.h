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
	/// 輸送機生成
	/// </summary>
	/// <returns></returns>
	Actor* CreateCargoEnemy();

	/// <summary>
	/// 武器を付与
	/// </summary>
	/// <param name="actor"></param>
	void GiveWeapon(Actor* actor, const EnemyData& data);

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
	void SelectCreateEnemy(const std::string& label);
	/// <summary>
	/// 読み込むjsonファイル選択(デバッグ用)
	/// </summary>
	void SelectJsonFile(const std::string& label);
	/// <summary>
	/// 統制するレベル(デバッグ用)
	/// </summary>
	void SelectLevel(const std::string& label);
	/// <summary>
	/// 作成したい武器の種類を選択(デバッグ用)
	/// </summary>
	void SelectWeaponType(int& selectedWeaponType, std::string label);
	/// <summary>
	/// 作成したい武器の種類を選択(デバッグ用)
	/// </summary>
	void SelectWeaponRarity();
	/// <summary>
	/// デバッグ用変数の情報作成
	/// </summary>
	void CreateDebugData();

	/// <summary>
	/// jsonファイル作成
	/// </summary>
	void CreateJsonData(LWP::Utility::JsonIO& json, EnemyData& data, const std::string& jsonName);
	/// <summary>
	/// 敵レベルのjsonファイル作成
	/// </summary>
	/// <param name="json"></param>
	/// <param name="data"></param>
	/// <param name="jsonName"></param>
	void CreateLevelJsonData(LWP::Utility::JsonIO& json, LevelParameter& data, const std::string& jsonName);
	/// <summary>
	/// 選択された武器のguiの表示
	/// </summary>
	void SelectEnemyDataGui(LWP::Utility::JsonIO& json, EnemyData& data);
	/// <summary>
	/// 選択されたレベルの表示
	/// </summary>
	/// <param name="json"></param>
	void SelectLevelGui(LWP::Utility::JsonIO& json, LevelParameter& data);

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 敵リストを取得
	/// </summary>
	/// <returns></returns>
	std::vector<Actor*> GetEnemyList() { return enemies_; }
	/// <summary>
	/// 特定の敵を探す
	/// </summary>
	/// <param name="actor"></param>
	/// <returns></returns>
	Actor* FindEnemy(Actor* actor) { 
		auto result = std::find(enemies_.begin(), enemies_.end(), actor);
		if (result == enemies_.end()) {
			return nullptr;
		}
		return *result;
	}
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 武器の装着位置を設定
	/// </summary>
	/// <param name="weapon"></param>
	void SetWeaponPos(IWeapon* weapon, int weaponSide);
#pragma endregion

private:// デバッグ用変数
	// ビヘイビアツリーの編集
	std::unique_ptr<BehaviorTreeGraph> btEditor_;

	// 敵の生成座標(デバッグ用)
	LWP::Math::Vector3 createPos_;
	// 敵の速度(デバッグ用)
	LWP::Math::Vector3 createVel_;

	// 作成する敵の名前
	//std::vector<int> createEnemyTypes_;

	// 作成できる敵の名前一覧
	std::vector<std::string> enemyTypePreview_;
	int selectCreateEnemyType_;
	// behaviorTreeのファイル名一覧
	std::vector<std::string> enemyBTFileNamePreview_;
	int selectBTFileName_;
	// レベル一覧
	std::vector<std::string> levelPreview_;
	int maxLevel_ = 10;
	int selectLevel_;

	// 作成できる武器種一覧
	std::vector<std::string> weaponTypePreview_;
	// 作成できる武器のレアリティ一覧
	std::vector<std::string> weaponRarityPreview_;
	// 作成したい武器のレアリティ
	int selectedWeaponRarity_;

	// 調整された敵のオリジナル(この敵の情報を参考にほかの敵を生成する)
	std::map<int, EnemyData> sampleEnemies_;
	// jsonファイル作成
	std::map<int, LWP::Utility::JsonIO> jsonDatas_;

	// 調整されたレベルのオリジナル
	std::map<int, LevelParameter> sampleLevels_;
	// レベルjsonファイル作成
	std::map<int, LWP::Utility::JsonIO> levelJsonDatas_;

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

