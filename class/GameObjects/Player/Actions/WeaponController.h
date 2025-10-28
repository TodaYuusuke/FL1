#pragma once
#include "IActions.h"
#include "Action/IAction.h"
#include "../../Weapon/IWeapon.h"
#include "../../Weapon/WeaponSlot.h"
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

public:// アクセサ
#pragma region Getter

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
		if (weapons_[WeaponSide::kLeft]->GetIsFullWeapon()) { return; }

		weapon->SetTranslation(LWP::Math::Vector3{ -1.0f, -0.5f, 2.0f });
		weapon->SetRotation(LWP::Math::Quaternion{ 0.0f,0.0f,0.0f,1.0f });
		weapon->SetParent(target_);
		weapons_[WeaponSide::kLeft]->AddWeapon(weapon);
	}
	/// <summary>
	/// 左肩の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetLeftShoulderWeapon(IWeapon* weapon) {
		if (weapons_[WeaponSide::kLeftShoulder]->GetIsFullWeapon()) { return; }

		weapon->SetTranslation(LWP::Math::Vector3{ -1.0f, 0.5f, 2.0f });
		weapon->SetRotation(LWP::Math::Quaternion{ 0.0f,0.0f,0.0f,1.0f });
		weapon->SetParent(target_);
		weapons_[WeaponSide::kLeftShoulder]->AddWeapon(weapon);
	}
	/// <summary>
	/// 右手の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightWeapon(IWeapon* weapon) {
		if (weapons_[WeaponSide::kRight]->GetIsFullWeapon()) { return; }

		weapon->SetTranslation(LWP::Math::Vector3{ 1.0f, -0.5f, 2.0f });
		weapon->SetRotation(LWP::Math::Quaternion{ 0.0f,0.0f,0.0f,1.0f });
		weapon->SetParent(target_);
		weapons_[WeaponSide::kRight]->AddWeapon(weapon);
	}
	/// <summary>
	/// 右肩の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightShoulderWeapon(IWeapon* weapon) {
		if (weapons_[WeaponSide::kRightShoulder]->GetIsFullWeapon()) { return; }

		weapon->SetTranslation(LWP::Math::Vector3{ 1.0f, 0.5f, 2.0f });
		weapon->SetRotation(LWP::Math::Quaternion{ 0.0f,0.0f,0.0f,1.0f });
		weapon->SetParent(target_);
		weapons_[WeaponSide::kRightShoulder]->AddWeapon(weapon);
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
	// 使用者のアドレス
	Actor* target_;

private:
	// 武器リスト
	std::map<WeaponSide, std::unique_ptr<WeaponSlot>> weapons_;
	// 現在所持している武器のマスク
	std::map<WeaponSide, unsigned int> currentWeaponMask_;

	// 武器の回収したい部位
	WeaponSide collectSide_;

	// デバッグ用の武器の持ち主
	Actor* debugOwner_;
};

