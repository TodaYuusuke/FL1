#pragma once
#include "WeaponConfig.h"
#include <Adapter.h>

class Player;
class Actor;
class IWorld;
class IWeapon;
class MachineGun;
class ShotGun;
class Rifle;
class Launcher;
class Missile;
class Melee;
/// <summary>
/// 武器管理クラス
/// </summary>
class WeaponManager final : public LWP::Utility::ISingleton<WeaponManager> {
	friend class LWP::Utility::ISingleton<WeaponManager>;// ISingletonをフレンドクラスにしてコンストラクタを呼び出せるように
public:
	// コンストラクタ
	WeaponManager();
	// デストラクタ
	~WeaponManager();

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
	/// 自機と武器との距離を確認
	/// </summary>
	void CheckPlayerToWeaponDistance();

	/// <summary>
	/// 指定した武器生成
	/// </summary>
	IWeapon* CreateSelectedWeapon(int weaponType);
	/// <summary>
	/// 指定した武器で使うモデル名
	/// </summary>
	/// <param name="weaponType"></param>
	/// <returns></returns>
	std::string ConvertWeaponModelName(int weaponType, int weaponRarity);

	/// <summary>
	/// 武器の種類を名前に変換
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	std::string ConvertWeaponTypeName(int type);
	/// <summary>
	/// 武器のレアリティを名前に変換
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	std::string ConvertWeaponRarityName(int rarity);

	/// <summary>
	/// コピー元となる武器を作成
	/// </summary>
	void CreateOriginWeapon();

private:
	/// <summary>
	/// マシンガン生成
	/// </summary>
	/// <returns></returns>
	MachineGun* CreateMachineGun();
	/// <summary>
	/// ショットガン生成
	/// </summary>
	/// <returns></returns>
	ShotGun* CreateShotGun();
	/// <summary>
	/// ライフル生成
	/// </summary>
	/// <returns></returns>
	Rifle* CreateRifle();
	/// <summary>
	/// ランチャー生成
	/// </summary>
	/// <returns></returns>
	Launcher* CreateLauncher();
	/// <summary>
	/// ミサイル生成
	/// </summary>
	/// <returns></returns>
	Missile* CreateMissile();
	/// <summary>
	/// 近接生成
	/// </summary>
	/// <returns></returns>
	Melee* CreateMelee();
	/// <summary>
	/// jsonファイル作成
	/// </summary>
	/// <param name="json"></param>
	/// <param name="fileName"></param>
	void CreateJsonData(LWP::Utility::JsonIO& json, WeaponData& data, const std::string& name);

	/// <summary>
	/// 選択された武器のguiの表示
	/// </summary>
	void SelectWeaponDataGui(LWP::Utility::JsonIO& json, WeaponData& data);
	/// <summary>
	/// 武器の種類を選択
	/// </summary>
	/// <param name="selectedWeaponType"></param>
	/// <param name="label"></param>
	void SelectWeaponType(int& selectedWeaponType, std::string label);
	/// <summary>
	/// 武器のレアリティを選択
	/// </summary>
	void SelectWeaponRarity(int& selectedWeaponRarity, std::string label);
	/// <summary>
	/// 弾を選択
	/// </summary>
	/// <param name="selectedType"></param>
	/// <param name="label"></param>
	void SelectBulletType(int& selectedBulletType, std::string label);

public:// アクセサ
	/// <summary>
	/// 武器を地面に落とす
	/// </summary>
	/// <param name="weapon"></param>
	/// <returns></returns>
	void DropWeapon(IWeapon* weapon);
	/// <summary>
	/// 落ちている武器を付与
	/// </summary>
	/// <param name="weapon">拾う武器</param>
	/// <param name="target">付与対象</param>
	void PickUpWeapon(IWeapon* weapon, Actor* target, int weaponSide = 0);
	/// <summary>
	/// 特定の武器を作成(受け取った武器のアドレスは解放禁止)
	/// </summary>
	/// <param name="weaponType"></param>
	/// <param name="weaponRarity"></param>
	/// <returns></returns>
	IWeapon* CreateWeapon(int weaponType, int weaponRarity);
	/// <summary>
	/// ランダムで武器を与える(受け取った武器のアドレスは解放禁止)
	/// </summary>
	/// <param name="weaponTypes"></param>
	/// <param name="weaponRarity"></param>
	/// <returns></returns>
	IWeapon* CreateRandomWeapon(const std::vector<int>& weaponTypes, const std::vector<int>& weaponRarity);

	/// <summary>
	/// 指定の武器を解放
	/// </summary>
	void DeleteWeapon(IWeapon* weapon);

public:// Getter,Setter
#pragma region Getter
	/// <summary>
	/// 作成できる武器種一覧を取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::string> GetWeaponTypePreview() { return weaponTypePreview_; }
	/// <summary>
	/// 作成できる武器のレアリティ一覧を取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::string> GetWeaponRarityPreview() { return weaponRarityPreview_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 地形等や自機の管理クラスを取得
	/// </summary>
	/// <param name="world"></param>
	void SetWorld(IWorld* world) { pWorld_ = world; }
	/// <summary>
	/// 自機のアドレスを取得
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ミニマップ登録用関数を設定
	/// </summary>
	/// <param name="player"></param>
	void SetMiniMapFunc(void (*func)(LWP::Math::Vector3)) { appendMiniMap_ = func; }
	
#pragma endregion

private:// 定数
	// jsonファイルのディレクトリパス
	const std::string kJsonDirectoryPath = "Weapons/";

private:
	// 武器を落とすときの速度ベクトル
	LWP::Math::Vector3 weaponDropVel = { 0.2f, 0.4f, 0.2f };
	
	// 武器回収範囲
	float pickUpWeaponRange = 3.0f;
	// 武器回収角度
	float pickUpWeaponAngle = 0.2f;

	LWP::Math::Vector3 maxPillarScale = { 1.5f,50.0f,1.0f };

private:// 外部から受け取る変数
	IWorld* pWorld_;
	Player* player_;

private:
	// 今から作られる武器の情報
	WeaponData createWeaponData_;

	// 武器リスト
	std::vector<IWeapon*> weapons_;

	// 拾える武器を画像で出す
	std::array<LWP::Primitive::NormalSprite, (int)WeaponType::kCount> pickUpWeaponSprite_;
	//コントローラーの種類
	enum ControllerUIType{
		kJoyPad,//xboxパッド
		kLever, //自作コン
		kCount  //使用禁止
	};
	inline static const std::array<std::string, (int)ControllerUIType::kCount> controllerUIName = {
				"Weapon/pickup_UI_pad.png",	// パッド
				"Weapon/pickup_UI_lever.png"		// レバー(自作コン)
	};
	std::array<LWP::Primitive::NormalSprite, (int)ControllerUIType::kCount> pickUpUISprite_;
	ControllerUIType controllerType_ = kJoyPad;
	// 拾える武器を画像のWorldTransform
	LWP::Primitive::NormalSprite samplePickUpWeaponSprite_;

	// 武器を生成する座標
	LWP::Math::Vector3 createPos_ = { 0.0f,10.0f,0.0f };

	int selectWeapon_;

	//ミニマップに追加する関数を格納
	void (*appendMiniMap_)(LWP::Math::Vector3);

	// --------- デバッグ用↓ --------- //
	// 各武器のコピー元の武器
	std::map<WeaponType, std::map<RarityType, WeaponData>> orizinWeaponData_;
	std::map<WeaponType, std::map<RarityType, LWP::Utility::JsonIO>> jsonDatas_;
	// 作成するコピー元の武器種
	int selectedOrizinWeaponType_;
	// 作成するコピー元のレアリティ
	int selectedOrizinRarityType_;

	// 調整できる弾一覧
	std::vector<std::string> bulletTypePreview_;
	// 作成できる武器種一覧
	std::vector<std::string> weaponTypePreview_;
	// 作成できる武器のレアリティ一覧
	std::vector<std::string> weaponRarityPreview_;
};

