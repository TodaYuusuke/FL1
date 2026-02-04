#pragma once
#include "ScoreUI.h"

class ScoreCounter
{
public:

	//インスタンス取得
	static ScoreCounter* GetInstance() {
		static ScoreCounter instance;
		return &instance;
	}

	void Update();

	void Reset() { sumScore_ = 0; drawScore_ = 0; };
	void ClearDrawScore() { drawScore_ = 0; };
	void SetMaxDrawScore() { drawScore_ = sumScore_; };

	void AddScore(int32_t score) { sumScore_ += score; };
	int32_t GetScore() { return sumScore_; };
	int32_t GetDrawScore() { return drawScore_; };
private:
	ScoreCounter() {};
	~ScoreCounter() {};
	ScoreCounter(const ScoreCounter&) = delete;
	ScoreCounter operator=(const ScoreCounter&) = delete;


	int32_t sumScore_=0;
	int32_t drawScore_=0;
	int32_t incrementFrame_ = 1;
};
