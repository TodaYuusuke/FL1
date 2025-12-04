#pragma once
#include "Device/IInputDevice.h"

class InputController {
public:
	// コンストラクタ
	InputController();
	// デストラクタ
	~InputController() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	std::vector<std::unique_ptr<IInputDevice>> device_;
};

