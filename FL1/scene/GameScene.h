#pragma once
#include "scene/IScene.h" 
#include "Adapter.h"

class GameScene final
	: public IScene {
public: // ** 純粋仮想関数の実体宣言 ** //

	// 初期化
	void Initialize();
	// 更新
	void Update();

private: // ** これより先に必要な処理や変数を記述 ** //

};