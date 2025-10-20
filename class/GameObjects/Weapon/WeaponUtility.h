#pragma once
#include "WeaponConfig.h"
#include <Adapter.h>

class IWeapon;
class MachineGun;
class ShotGun;
class Rifle;
class Launcher;
class Missile;
class Melee;
class WeaponUtility {
public:
	// コンストラクタ
	WeaponUtility();
	// デストラクタ
	~WeaponUtility() = default;

	//void Update();
	
	/// <summary>
	/// 
	/// </summary>
	void DrawGui();

	/// <summary>
	/// 武器生成
	/// </summary>
	void CreateWeapon();

private:
	void ChangeWeapon();

	MachineGun* CreateMachineGun();
	ShotGun* CreateShotGun();
	Rifle* CreateRifle();
	Launcher* CreateLauncher();
	Missile* CreateMissile();
	Melee* CreateMelee();

private:
	WeaponData data_;

	// 作成できる武器の名前一覧
	std::vector<std::string> weaponPreview_;
	int selectWeapon_;
};