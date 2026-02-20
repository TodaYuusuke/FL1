#include "RankingSystem.h"

void RankingSystem::Initialize() {

	// 管理クラスの調整項目
	json_.Init(kJsonDirectoryPath + "Ranking.json")
		.AddValue<LWP::Math::Vector2>("Anchor", &anchor_)
		.AddValue<LWP::Math::Vector2>("Offset", &offset_)
		.AddValue<LWP::Math::Vector2>("RankOffset", &rankOffset_)
		.AddValue<LWP::Math::Vector2>("RankNumOffset", &rankNumOffset_)
		.AddValue<LWP::Math::Vector2>("Ranking", &rankingPos_)
		
		.CheckJsonFile();

	rankingJson_.Init(kJsonDirectoryPath + "RankingData.json")
		.BeginGroup("Ranking")
		.AddValue<int>("Num0", &scoreNums_[0])
		.AddValue<int>("Num1", &scoreNums_[1])
		.AddValue<int>("Num2", &scoreNums_[2])
		.AddValue<int>("Num3", &scoreNums_[3])
		.AddValue<int>("Num4", &scoreNums_[4])
		.AddValue<int>("Num5", &scoreNums_[5])
		.AddValue<int>("Num6", &scoreNums_[6])
		.AddValue<int>("Num7", &scoreNums_[7])
		.AddValue<int>("Num8", &scoreNums_[8])
		.AddValue<int>("Num9", &scoreNums_[9])
		.EndGroup()

		.CheckJsonFile();

	rankingSprite_.LoadTexture("Result/ranking.png");
	rankingSprite_.worldTF.translation = {rankingPos_.x,rankingPos_.y,0.0f};
	rankingSprite_.worldTF.scale = {1.5f,1.5f,1.0f};

	for (size_t i = 0; i < num_;i++) {
		scores_[i] = std::make_unique<ScoreUI>();
		scores_[i]->Initialize(9);
		scores_[i]->SetIsScoreDisplay(false);
		LWP::Math::Vector2 pos = LWP::Math::Vector2{ anchor_.x + float(i / 5) * offset_.x,anchor_.y + float(i % 5) * offset_.y };
		scores_[i]->SetCenter(pos);

		rankSprites_[i].LoadTexture("Result/rank.png");
		rankSprites_[i].worldTF.translation = {pos.x + rankOffset_.x,pos.y + rankOffset_.y,0.0f};

		rankNums_[i] = std::make_unique<ScoreUI>();
		rankNums_[i]->Initialize(2);
		rankNums_[i]->SetIsScoreDisplay(false);
		rankNums_[i]->SetScore(i+1);
	}

	nowRanking_ = -1;
}

void RankingSystem::Update() {
	rankingSprite_.worldTF.translation = { rankingPos_.x,rankingPos_.y,0.0f };
	if (nowRanking_ >=0) {
		scores_[nowRanking_]->SetColor({200,200,30,255});
	}
	for (size_t i = 0; i < num_; i++) {
		LWP::Math::Vector2 pos = LWP::Math::Vector2{ anchor_.x + float(i / 5) * offset_.x,anchor_.y + float(i % 5) * offset_.y };
		scores_[i]->SetScore(scoreNums_[i]);
		scores_[i]->SetCenter(pos);
		scores_[i]->Update();

		rankSprites_[i].worldTF.translation = { pos.x + rankOffset_.x,pos.y + rankOffset_.y,0.0f };

		rankNums_[i]->SetCenter({pos.x + rankNumOffset_.x,pos.y + rankNumOffset_.y });
		rankNums_[i]->Update();
	}
}


void RankingSystem::CalcRanking() {

	//ランキングを更新
	for (size_t i = 0; i < num_;i++) {
		if (scoreNums_[i] < newScore_) {
			for (size_t j = num_-1; j > i;j--) {
				scoreNums_[j] = scoreNums_[j - 1];
			}
			scoreNums_[i] = newScore_;
			nowRanking_ = i;
			rankingJson_.Save();
			break;
		}
	}
}


void RankingSystem::DebugGUI() {
	if (ImGui::BeginTabItem("Ranking")) {
		json_.DebugGUI();
		ImGui::EndTabItem();
	}
}