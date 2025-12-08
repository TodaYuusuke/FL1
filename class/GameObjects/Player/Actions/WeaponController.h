#pragma once
#include "IActions.h"
#include "Action/IAction.h"
#include "../System/WeaponSkill/WeaponSkill.h"
#include "../../Weapon/IWeapon.h"
#include "../../Weapon/WeaponSlot.h"
#include "../../../Componets/Input/VirtualController.h"
#include "../../UI/NumPlane.h"
#include <memory>
#include <vector>

class LeadingSystem;
/// <summary>
/// メイン、サブ武器やリロード、ADS等の処理
/// </summary>
class WeaponController : public IActions {
public:
	// コンストラクタ
	WeaponController(LeadingSystem* leadingSystem, Actor* target);
	// デストラクタ
	~WeaponController() override;

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
	void DebugGui() override;

private:
	/// <summary>
	/// 入力処理
	/// </summary>
	void InputHandle();

	/// <summary>
	/// 装着時の武器設定
	/// </summary>
	/// <param name="weapon"></param>
	/// <param name="q"></param>
	/// <param name="weaponPos"></param>
	/// <param name="weaponSide"></param>
	void SettingWeapon(IWeapon* weapon, LWP::Math::Quaternion q, LWP::Math::Vector3 weaponPos, WeaponSide weaponSide) {
		if (weapons_[weaponSide]->GetIsFullWeapon()) { return; }

		weapon->SetWeaponSide((int)weaponSide);
		weapon->Init();
		weapon->SetTranslation(weaponPos);
		weapon->SetRotation(q);
		weapon->SetParent(target_);
		weapons_[weaponSide]->AddWeapon(weapon);
	}

public:// アクセサ
#pragma region Getter
	/// <summary>
	/// 武器由来の速度を取得
	/// </summary>
	/// <returns></returns>
	LWP::Math::Vector3 GetWeaponVelocity() { return weaponVel_; }
#pragma endregion

#pragma region Setter
	/// <summary>
	/// 武器を設定
	/// </summary>
	/// <param name="weapon"></param>
	void SetWeapon(IWeapon* weapon);
	/// <summary>
	/// 左手の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetLeftWeapon(IWeapon* weapon) {
		SettingWeapon(weapon, LWP::Math::Quaternion{ 0,0,0,1 }, centerPos_ + LWP::Math::Vector3{ -1.0f, -0.7f, 3.0f }, WeaponSide::kLeft);
	}
	/// <summary>
	/// 左肩の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetLeftShoulderWeapon(IWeapon* weapon) {
		SettingWeapon(weapon, LWP::Math::Quaternion{ 0,0,0,1 }, centerPos_ + LWP::Math::Vector3{ -1.25f, 0.5f, 3.0f }, WeaponSide::kLeftShoulder);
	}
	/// <summary>
	/// 右手の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightWeapon(IWeapon* weapon) {
		SettingWeapon(weapon, LWP::Math::Quaternion{ 0,0,0,1 }, centerPos_ + LWP::Math::Vector3{ 1.0f, -0.7f, 3.0f }, WeaponSide::kRight);
	}
	/// <summary>
	/// 右肩の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightShoulderWeapon(IWeapon* weapon) {
		SettingWeapon(weapon, LWP::Math::Quaternion{ 0,0,0,1 }, centerPos_ + LWP::Math::Vector3{ 1.25f, 0.5f, 3.0f }, WeaponSide::kRightShoulder);
	}

	/// <summary>
	/// デバッグ用の武器の持ち主を設定
	/// </summary>
	/// <param name="player"></param>
	void SetDebugWeaponOwner(Actor* owner) { debugOwner_ = owner; }

	/// <summary>
	/// 中心座標を設定
	/// </summary>
	/// <param name="centerPos"></param>
	void SetCenterDist(const LWP::Math::Vector3& centerPos) { centerPos_ = centerPos; }
	/// <summary>
	/// 武器由来の速度を設定
	/// </summary>
	/// <param name="velocity"></param>
	void SetWeaponVelocity(const LWP::Math::Vector3& velocity) { weaponVel_ = velocity; }
#pragma endregion

private:
	VirtualController* vCon_;
	// 偏差射撃計算機能
	LeadingSystem* pLeadingSystem_;
	// 使用者のアドレス
	Actor* target_;
	// 中心座標
	LWP::Math::Vector3 centerPos_;

private:
	// 武器リスト
	std::map<WeaponSide, std::unique_ptr<WeaponSlot>> weapons_;
	// 現在所持している武器のマスク
	std::map<WeaponSide, unsigned int> currentWeaponMask_;

	// 各武器の練度
	std::unique_ptr<WeaponSkill> weaponSkills_;

	// 武器の回収したい部位
	WeaponSide collectSide_;

	// 武器由来の速度
	LWP::Math::Vector3 weaponVel_{ 0.0f,0.0f,0.0f };

	// デバッグ用の武器の持ち主
	Actor* debugOwner_;

private://UI表示
	// jsonファイルのディレクトリパス
	const std::string kJsonDirectoryPath = "WeaponUI/";
	//所有武器用の平面
	std::map<WeaponSide, std::array<LWP::Primitive::NormalSurface, (int)WeaponType::kCount>> weaponSurfaces_;
	std::map<WeaponSide, LWP::Primitive::NormalSurface> sampleWeaponSurface_;

	//弾数表示
	std::map < WeaponSide, std::unique_ptr<NumPlane>> bulletNums_;
	LWP::Primitive::NormalSurface sampleBulletSurface_;
	int kBulletNumDigit_ = 3;

	//コックピット表示
	LWP::Resource::RigidModel cockpit_;
	LWP::Utility::JsonIO json_;
};

