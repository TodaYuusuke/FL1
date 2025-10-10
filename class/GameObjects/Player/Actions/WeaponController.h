#pragma once
#include "IActions.h"
#include "Action/IAction.h"
#include "../../Weapon/IWeapon.h"
#include <memory>
#include <vector>

class Player;
/// <summary>
/// メイン、サブ武器やリロード、ADS等の処理
/// </summary>
class WeaponController : public IActions {
public:
	// コンストラクタ
	WeaponController();
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
	void SetLeftWeapon(IWeapon* weapon) { weapons_[WeaponSide::kLeft] = weapon; }
	/// <summary>
	/// 右側の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightWeapon(IWeapon* weapon) { weapons_[WeaponSide::kRight] = weapon; }
	
	/// <summary>
	/// デバッグ用の武器の持ち主を設定
	/// </summary>
	/// <param name="player"></param>
	void SetDebugWeaponOwner(ICharacter* owner) { debugOwner_ = owner; }
#pragma endregion

private:

private:
	// 行動処理をまとめたリスト
	//std::map<WeaponSide, std::unique_ptr<IAction>> actions_;

	// 武器リスト
	std::map<WeaponSide, IWeapon*> weapons_;
	// 現在所持している武器のマスク
	std::map<WeaponSide, unsigned int> currentWeaponMask_;

	// デバッグ用の武器の持ち主
	ICharacter* debugOwner_;
};

