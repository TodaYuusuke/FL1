#pragma once
#include <Adapter.h>
#include "../../../Componets/EaseMachine.h"
#include "../../../Componets/SerialAnimation.h"

class Player;
class EnemyManager;
class BaseTutorialData {
public:
	// コンストラクタ
	BaseTutorialData();
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

protected:
	/// <summary>
	/// 開始演出
	/// </summary>
	void StartEffect();
	/// <summary>
	/// 終了演出
	/// </summary>
	void FinishEffect();
	/// <summary>
	/// 次の説明に移動
	/// </summary>
	bool NextGuide(float nextGuideInterval);

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
	LWP::Primitive::NormalSprite introFont_;
	LWP::Primitive::NormalSprite successFont_;

	// 連番アニメーション
	std::unique_ptr<SerialAnimation> serialAnim_;

	EaseMachine flashingEffect_;
	int flashAlpha_;

	float nextGuideInterval_;
	bool isNextGuide_ = false;
	bool isFinish_ = false;
};

