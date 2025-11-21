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

	void Reset() { sumScore_ = 0; };

	void AddScore(int32_t score) { sumScore_ += score; };
	int32_t GetScore() { return sumScore_; };

private:
	ScoreCounter() {};
	~ScoreCounter() {};
	ScoreCounter(const ScoreCounter&) = delete;
	ScoreCounter operator=(const ScoreCounter&) = delete;


	int32_t sumScore_=0;


};
