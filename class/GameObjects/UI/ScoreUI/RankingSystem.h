#pragma once
#include <Adapter.h>
#include "ScoreUI.h"

class RankingSystem
{
public:
	RankingSystem() {};
	~RankingSystem() {};

	void Initialize();

	void Update();

	void CalcRanking();

	//新規スコアを格納
	void SetNewScore(int score) { newScore_ = score; };

	void SetIsVisible(bool is) {
		for (size_t i = 0; i < num_; i++) {
			scores_[i]->SetIsScoreDisplay(is);
			rankSprites_[i].isActive = is;
			rankNums_[i]->SetIsScoreDisplay(is);
			rankingSprite_.isActive = is;
		}
	};

	void DebugGUI();

private:

	const std::string kJsonDirectoryPath = "Result/";
	LWP::Utility::JsonIO json_;
	LWP::Utility::JsonIO rankingJson_;

	static const size_t num_ = 10;//件数
	std::array<std::unique_ptr<ScoreUI>, num_> scores_{};
	std::array<std::unique_ptr<ScoreUI>, num_> rankNums_{};
	std::array<LWP::Primitive::NormalSprite, num_> rankSprites_{};
	LWP::Primitive::NormalSprite rankingSprite_{};
	int scoreNums_[10]={0};
	int newScore_;

	LWP::Math::Vector2 anchor_ = {};
	LWP::Math::Vector2 offset_ = {};
	LWP::Math::Vector2 rankOffset_ = {};
	LWP::Math::Vector2 rankNumOffset_ = {};
	LWP::Math::Vector2 rankingPos_ = {};
	int nowRanking_=-1;
};
