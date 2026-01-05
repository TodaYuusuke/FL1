#pragma once
#include "../../Componets/BehaviourTree/Actor/Actor.h"
#include "../Weapon/WeaponConfig.h"
#include "EnemyConfig.h"
#include <Adapter.h>
#include <filesystem>

class IWorld;
/// <summary>
/// 敵管理クラス
/// </summary>
class EnemyManager {
public:
	// 敵発生情報
	struct SpawnData {
		int spawnNum;				// 敵生成数
		float nextSpawnTime = 6.0f;	// 次の敵が強制的に出るまでの時間
	};

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
	/// ゲームシーンの最後に呼ぶ処理
	/// </summary>
	void EndFrame();
	/// <summary>
	/// 調整項目
	/// </summary>
	void DebugGui();

private:// 敵の生成関数
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
	/// 選ばれた敵を作成(デバッグ用)
	/// </summary>
	Actor* CreateEnemy();

private:// 敵に持たせる武器に関する関数
	/// <summary>
	/// 武器を付与
	/// </summary>
	/// <param name="actor"></param>
	void GiveWeapon(Actor* actor, const EnemyData& data);

private:// 敵出現用の関数
	/// <summary>
	/// 敵出現
	/// </summary>
	void SpawnEnemy();
	/// <summary>
	/// 敵の出現位置をランダムで生成
	/// </summary>
	void CreateRandomSpawnPos();
				
private:// ******** デバッグ、エディタ用の関数群↓ ******** //
	/// <summary>
	/// テスト敵生成(デバッグ用)
	/// </summary>
	Actor* CreateTestEnemy();

	/// <summary>
	/// ComboGuiの表示
	/// </summary>
	/// <param name="list"></param>
	/// <param name="selectedType"></param>
	/// <param name="label"></param>
	void SelectType(std::vector<std::string> list, int& selectedType, std::string label);
	void SelectType(std::vector<std::string> list, int& selectedType, std::string label, bool& isClickCombo);
	/// <summary>
	/// 作成する敵を選択(デバッグ用)
	/// </summary>
	void SelectEnemyType(std::map<int, EnemyData> data, int& selectType, const std::string& label);

	/// <summary>
	/// デバッグ用変数の作成
	/// </summary>
	void CreateDebugData();
	/// <summary>
	/// 敵単体のjsonファイル作成
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
	void SelectEnemyGui(LWP::Utility::JsonIO& json, EnemyData& data);
	/// <summary>
	/// 選択されたレベルのguiの表示
	/// </summary>
	/// <param name="json"></param>
	void SelectLevelGui(LWP::Utility::JsonIO& json, LevelParameter& data);

private:// 配置エディタで使用する関数
	/// <summary>
	/// 配置済み敵のguiの表示
	/// </summary>
	void SpawnedEnemiesGui();
	/// <summary>
	/// 敵配置guiの表示
	/// </summary>
	void SpawnEnemyGui();

	/// <summary>
	/// 生成順に整列
	/// </summary>
	void SortSpawnTime();

	/// <summary>
	/// 敵の配置jsonファイルの読み込みボタン
	/// </summary>
	/// <param name="fileName"></param>
	void LoadSpawnJsonButton(const std::string& fileName);
	/// <summary>
	/// 敵の配置をjsonファイルから読み込む処理
	/// </summary>
	/// <param name="fileName"></param>
	void LoadSpawnJson(const std::string& fileName);
	/// <summary>
	/// 敵の配置jsonファイルの保存ボタン
	/// </summary>
	void ExportSpawnJsonButton();
	/// <summary>
	/// 敵の配置をjsonファイルに出力する処理
	/// </summary>
	/// <param name="fileName"></param>
	void ExportSpawnJson(const std::string& fileName);

	/// <summary>
	/// exeからのディレクトリパスを取得
	/// </summary>
	/// <returns></returns>
	std::filesystem::path GetExeDir();
	/// <summary>
	/// 指定したフォルダに入っているjsonファイル群の取得
	/// </summary>
	/// <param name="folderPath"></param>
	/// <returns></returns>
	std::vector<std::string> GetFileNames(const std::string& folderPath);

	bool Contains(const std::string& str, const std::string& target) {
		return str.find(target) != std::string::npos;
	}

		// ******** デバッグ、エディタ用の関数群↑ ******** //

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
	/// <summary>
	/// 倒した敵の数を取得
	/// </summary>
	/// <returns></returns>
	int GetKillCount() { return killCount_; }
	/// <summary>
	/// 敵が解放された瞬間を取得
	/// </summary>
	/// <returns></returns>
	bool GetTriggerDeleteEnemy() { return isTriggerDelete_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 武器の装着位置を設定
	/// </summary>
	/// <param name="weapon"></param>
	void SetWeaponPos(Actor* actor, IWeapon* weapon, int weaponSide);
#pragma endregion

private:// 定数
	// jsonファイルのディレクトリパス
	const std::string kJsonDirectoryPath = "Enemies/";
	const std::string kLevelJsonDirectoryPath = "Enemies/Levels/";

private:// デバッグ用変数
	std::vector<EnemySpawnData> spawnDatas_;
	LWP::Resource::SkinningModel spawnModel_;

	// ビヘイビアツリーの編集
	std::unique_ptr<BehaviorTreeGraph> btEditor_;

	// 敵の速度(デバッグ用)
	LWP::Math::Vector3 createVel_;

	// 作成できる敵の名前一覧
	std::vector<std::string> enemyTypePreview_;
	int selectCreateEnemyType_;
	// behaviorTreeのファイル名一覧
	std::vector<std::string> enemyBTFileNamePreview_;
	int modifyBTFile_;
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

	// ランダム出現開始
	//bool isRandomSpawn_ = true;
	EnemySpawnType spawnType_ = EnemySpawnType::kWave;

private:// 外部から受け取る変数
	IWorld* pWorld_;

private:// 調整項目
	// ドローンの浮遊標高
	float dronefloatHeight_ = 10.0f;

	// 一度に出現する敵の最小数
	int minSameSpawnNum_ = 4;
	// 一度に出現する敵の最大数
	int maxSameSpawnNum_ = 8;
	// 敵が一度に存在できる数
	int limitSpawnNum_ = 10;
	// 自機を中心とした生成禁止範囲
	float invalidSpawnRange_ = 25.0f;

	LWP::Utility::JsonIO json_;

private:
	// 敵リスト
	std::vector<Actor*> enemies_;

	// 敵の生成座標
	LWP::Math::Vector3 createPos_;

	SpawnData spawnData_;
	float currentFrame_;
	float spawnInterval_;

	// 敵の死亡回数
	int killCount_;
	// 生成時の識別番号
	int createID_;

	// 敵を解放した瞬間
	bool isTriggerDelete_;
};

