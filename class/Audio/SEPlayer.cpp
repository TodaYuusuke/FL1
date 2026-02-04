#include "SEPlayer.h"

SEPlayer::SEPlayer()
{
	// マップに対してチャンネル追加
	for (int i = 0; i < LWP::AudioConfig::SEChannels::Count; i++) {
		seMap_.try_emplace(i);
	}
}

SEPlayer::~SEPlayer()
{
	// 全ての効果音の停止
	for (auto& [channel, players] : seMap_) {
		for (auto& player : players) {
			player.Stop();
		}
	}

	// 配列クリア
	seMap_.clear();
}

void SEPlayer::Update() {

	// イテレータ用意
	std::map<int, std::vector<AudioPlayer>>::iterator mapIt;
	// 配列内の効果音が停止している場合配列から除外する
	for (mapIt = seMap_.begin(); mapIt != seMap_.end(); mapIt++) {
		// オーディオプレイヤー取得
		std::vector<AudioPlayer>& players = mapIt->second;

		// 再生中のプレイヤーを更新
		for (AudioPlayer& player : players)
		{
			if (player.GetIsPlaying())
			{
				player.Update();
			}
		}

		// 再生が終わったプレイヤーの除外
		players.erase(
			std::remove_if(
				players.begin(),
				players.end(),
				[](AudioPlayer& player) {
					return !player.GetIsPlaying();
				}
			),
			players.end()
		);
	}

}

void SEPlayer::PlaySE(const std::string& filePath, float volume, int channelID)
{
	// 最終的なパスを求める
	std::string path = "SE/" + filePath;

	// 新規オーディオプレイヤーの生成と再生
	AudioPlayer audioPlayer(path, volume, false, &masterVolume_);
	audioPlayer.Play();
	// 指定されたチャンネルIDに追加
	seMap_[channelID].push_back(audioPlayer);
}

AudioPlayer& SEPlayer::PlaySE(const std::string& filePath, float volume, int channelID, const LWP::Math::Vector3& pos)
{
	// もしリスナーがセットされていない状態でこの関数が呼び出された場合、通常の再生関数で呼び出し早期リターン
	if (listener_ == nullptr) { PlaySE(filePath, volume, channelID); return seMap_[channelID].back();
	}

	// 最終的なパスを求める
	std::string path = "SE/" + filePath;

	// 新規オーディオプレイヤーの生成と再生
	AudioPlayer audioPlayer(path, volume, false, &masterVolume_);
	// リスナーを渡す
	audioPlayer.SetListener(listener_);
	audioPlayer.SetEmitPos(pos);
	audioPlayer.Play();
	// 指定されたチャンネルIDに追加
	seMap_[channelID].push_back(audioPlayer);
	// 再生したものを返す
	return seMap_[channelID].back();
}

void SEPlayer::PlayRandomSE(const std::string& filePath, int maxRandomCount, float volume, int channelID)
{
	// 名前を' | 'で分割する
	std::vector<std::string> splitName = LWP::Utility::Split(filePath, '.');

	// 最終的なパスを求める
	std::string path = "SE/" + splitName[0] + std::to_string(LWP::Utility::Random::GenerateInt(1, maxRandomCount)) + "." + splitName[1];

	// 新規オーディオプレイヤーの生成と再生
	AudioPlayer audioPlayer(path, volume, false, &masterVolume_);
	audioPlayer.Play();
	// 指定されたチャンネルIDに追加
	seMap_[channelID].push_back(audioPlayer);
}

AudioPlayer& SEPlayer::PlayRandomSE(const std::string& filePath, int maxRandomCount, float volume, int channelID, const LWP::Math::Vector3& pos)
{
	// もしリスナーがセットされていない状態でこの関数が呼び出された場合、通常の再生関数で呼び出し早期リターン
	if (listener_ == nullptr) { PlaySE(filePath, volume, channelID); return seMap_[channelID].back(); }

	// 名前を' | 'で分割する
	std::vector<std::string> splitName = LWP::Utility::Split(filePath, '.');

	// 最終的なパスを求める
	std::string path = "SE/" + splitName[0] + std::to_string(LWP::Utility::Random::GenerateInt(1, maxRandomCount)) + "." + splitName[1];

	// 新規オーディオプレイヤーの生成と再生
	AudioPlayer audioPlayer(path, volume, false, &masterVolume_);
	// リスナーを渡す
	audioPlayer.SetListener(listener_);
	audioPlayer.SetEmitPos(pos);
	audioPlayer.Play();
	// 指定されたチャンネルIDに追加
	seMap_[channelID].push_back(audioPlayer);
	// 再生したものを返す
	return seMap_[channelID].back();
}

void SEPlayer::SetVolume(const float volume, const int channelID)
{
	// 効果音配列取得
	std::vector<AudioPlayer>& players = seMap_[channelID];

	// 取得した配列内に何も入っていなかったら早期リターン
	if (players.empty()) { return; }

	// 配列内の全ての効果音の音量を調節
	for (int i = 0; i < players.size(); i++) {
		players[i].SetVolume(volume);
	}
}
