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
	~WeaponManager() = default;

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
	std::string SelectWeaponModelName(int weaponType);

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
	void CreateOrizinWeapon();

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

public:// アクセサ
	/// <summary>
	/// 武器を地面に落とす
	/// </summary>
	/// <param name="weapon"></param>
	/// <returns></returns>
	void DropWeapon(IWeapon* weapon);
	/// <summary>
	/// 武器の付与
	/// </summary>
	/// <param name="weapon"></param>
	/// <returns></returns>
	void GiveWeapon(IWeapon* weapon, Actor* target);
	/// <summary>
	/// 特定の武器を与える(受け取った武器のアドレスは解放禁止)
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

public:// Getter,Setter
#pragma region Getter

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
#pragma endregion

private:// 外部から受け取る変数
	IWorld* pWorld_;
	Player* player_;

private:
	// 今から作られる武器の情報
	WeaponData createWeaponData_;

	// 武器リスト
	std::vector<IWeapon*> weapons_;

	// 武器を生成する座標
	LWP::Math::Vector3 createPos_;

	int selectWeapon_;

	// --------- デバッグ用↓ --------- //
	// 各武器のコピー元の武器
	std::map<WeaponType, std::map<RarityType, IWeapon*>> orizinWeaponData_;
	// 作成するコピー元の武器種
	int selectedOrizinWeaponType_;
	// 作成するコピー元のレアリティ
	int selectedOrizinRarityType_;

	// 作成できる武器種一覧
	std::vector<std::string> weaponTypePreview_;
	// 作成できる武器のレアリティ一覧
	std::vector<std::string> weaponRarityPreview_;
};

