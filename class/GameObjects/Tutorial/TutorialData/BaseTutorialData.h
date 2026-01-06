#pragma once
#include <Adapter.h>

class Player;
class EnemyManager;
class BaseTutorialData {
public:
	// コンストラクタ
	BaseTutorialData() = default;
	// デストラクタ
	virtual ~BaseTutorialData() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() {}
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {}

public:
#pragma region Getter
	/// <summary>
	/// 終了しているかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsFinish() { return isFinish_; }
#pragma endregion

#pragma region Setter
#pragma endregion


protected:// 外部から受け取る変数
	Player* player_;
	EnemyManager* enemyManager_;

protected:
	bool isFinish_ = false;
};

