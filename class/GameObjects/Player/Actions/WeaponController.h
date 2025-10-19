#pragma once
#include "IActions.h"
#include "Action/IAction.h"
#include "../../Weapon/IWeapon.h"
#include "../../Weapon/WeaponSlot.h"
#include <memory>
#include <vector>

class Player;
class LeadingSystem;
/// <summary>
/// メイン、サブ武器やリロード、ADS等の処理
/// </summary>
class WeaponController : public IActions {
public:
	// コンストラクタ
	WeaponController(LeadingSystem* leadingSystem);
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

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter
	/// <summary>
	/// 左側の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetLeftWeapon(std::unique_ptr<IWeapon> weapon) {
		weapons_[WeaponSide::kLeft]->AddWeapon(std::move(weapon));
	}
	/// <summary>
	/// 右側の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightWeapon(std::unique_ptr<IWeapon> weapon) {
		weapons_[WeaponSide::kRight]->AddWeapon(std::move(weapon));
	}

	/// <summary>
	/// デバッグ用の武器の持ち主を設定
	/// </summary>
	/// <param name="player"></param>
	void SetDebugWeaponOwner(Actor* owner) { debugOwner_ = owner; }
#pragma endregion

private:
	// 偏差射撃計算機能
	LeadingSystem* pLeadingSystem_;

private:
	// 武器リスト
	//std::map<WeaponSide, IWeapon*> weapons_;
	std::map<WeaponSide, std::unique_ptr<WeaponSlot>> weapons_;
	// 現在所持している武器のマスク
	std::map<WeaponSide, unsigned int> currentWeaponMask_;

	// デバッグ用の武器の持ち主
	Actor* debugOwner_;
};

