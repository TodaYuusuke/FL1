#pragma once
#include "../ICharacter.h"
#include "Actions/MoveController.h"
#include "Actions/WeaponController.h"
#include <Adapter.h>

class Player : public ICharacter {
public:
	// コンストラクタ
	Player(LWP::Object::Camera* camera);
	// デストラクタ
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="delta_time"></param>
	void Update() override;
	/// <summary>
	/// 調整項目
	/// </summary>
	void DrawGui() override;

public:// アクセサ
#pragma region Getter

#pragma endregion

#pragma region Setter
	/// <summary>
	/// 左側の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetLeftWeapon(IWeapon* weapon) { weaponController_->SetLeftWeapon(weapon); }
	/// <summary>
	/// 右側の武器を設定する
	/// </summary>
	/// <param name="weapon"></param>
	void SetRightWeapon(IWeapon* weapon) { weaponController_->SetRightWeapon(weapon); }
#pragma endregion

private:
	// 移動行動系の処理
	std::unique_ptr<MoveController> moveController_;
	// 武器の処理
	std::unique_ptr<WeaponController> weaponController_;
};