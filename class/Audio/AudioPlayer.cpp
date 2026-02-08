#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
	// マップに対してチャンネル追加
	for (int i = 0; i < LWP::AudioConfig::SEChannels::Count; i++) {
		audioMap_.try_emplace(i);
	}
}

AudioPlayer::~AudioPlayer()
{
	// 配列クリア
	audioMap_.clear();
}

void AudioPlayer::Update() {

	// イテレータ用意
	std::map<int, std::deque<std::unique_ptr<Sound>>>::iterator mapIt;
	// 配列内の効果音が停止している場合配列から除外する
	for (mapIt = audioMap_.begin(); mapIt != audioMap_.end(); mapIt++) {
		// オーディオプレイヤー取得
		std::deque<std::unique_ptr<Sound>>& players = mapIt->second;

		// 再生中のプレイヤーを更新
		for (auto& player : players)
		{
			if (player->GetIsPlaying()) {
				player->Update();
			}
		}

		// 再生が終わったプレイヤーの除外
		players.erase(
			std::remove_if(
				players.begin(),
				players.end(),
				[&](auto& player) {
					if (!player->GetIsPlaying()) {
						// IDマップからIDを除外
						idMap_.erase(player->GetInstanceID());
						return true;
					}
					return false;
				}
			),
			players.end()
		);
	}

}

int32_t AudioPlayer::PlayAudio(const std::string& filePath, float volume, int channelID, const bool isLoop)
{
	// 新規オーディオプレイヤーの生成
	std::unique_ptr<Sound> audioPlayer = std::make_unique<Sound>(filePath, volume, isLoop, &masterVolume_);
	// プレイヤーのID作成
	uint32_t id = nextID_++;
	audioPlayer->SetInstanceID(id);

	// 再生
	audioPlayer->Play();

	// 配列に入れた実体への参照を入れる
	idMap_[id] = audioPlayer.get();
	audioMap_[channelID].push_back(std::move(audioPlayer));
	// IDを返す
	return id;
}

int32_t AudioPlayer::PlayAudio(const std::string& filePath, float volume, int channelID, const LWP::Math::Vector3& pos, const bool isLoop)
{
	// もしリスナーがセットされていない状態でこの関数が呼び出された場合、通常の再生関数で呼び出し早期リターン
	if (listener_ == nullptr) { return PlayAudio(filePath, volume, channelID); }

	// 新規オーディオプレイヤーの生成
	std::unique_ptr<Sound> audioPlayer = std::make_unique<Sound>(filePath, volume, isLoop, &masterVolume_);
	// プレイヤーのID作成
	uint32_t id = nextID_++;
	audioPlayer->SetInstanceID(id);

	// 再生
	audioPlayer->SetListener(listener_);
	audioPlayer->SetEmitPos(pos);
	audioPlayer->Play();

	// 配列に入れた実体への参照を入れる
	idMap_[id] = audioPlayer.get();
	audioMap_[channelID].push_back(std::move(audioPlayer));
	// IDを返す
	return id;
}

int32_t AudioPlayer::PlayRandomAudio(const std::string& filePath, int maxRandomCount, float volume, int channelID)
{
	// 名前を' | 'で分割する
	std::vector<std::string> splitName = LWP::Utility::Split(filePath, '.');

	// 最終的なパスを求める
	std::string path = splitName[0] + std::to_string(LWP::Utility::Random::GenerateInt(1, maxRandomCount)) + "." + splitName[1];

	// 求めたパスで再生
	return PlayAudio(path, volume, channelID);
}

int32_t AudioPlayer::PlayRandomAudio(const std::string& filePath, int maxRandomCount, float volume, int channelID, const LWP::Math::Vector3& pos)
{
	// もしリスナーがセットされていない状態でこの関数が呼び出された場合、通常の再生関数で呼び出し早期リターン
	if (listener_ == nullptr) { return PlayAudio(filePath, volume, channelID); }

	// 名前を' | 'で分割する
	std::vector<std::string> splitName = LWP::Utility::Split(filePath, '.');

	// 最終的なパスを求める
	std::string path = splitName[0] + std::to_string(LWP::Utility::Random::GenerateInt(1, maxRandomCount)) + "." + splitName[1];

	// 求めたパスで再生
	return PlayAudio(path, volume, channelID, pos);
}

Sound* AudioPlayer::GetAudioPlayer(const uint32_t id)
{
	// プレイヤーが存在するかどうか調べる
	auto player = idMap_.find(id);
	if (player == idMap_.end()) {
		// 存在しなければnullptr
		return nullptr;
	}

	// 存在した場合それを返す
	return player->second;
}

void AudioPlayer::SetChannelVolume(const float volume, const int channelID)
{
	// 効果音配列取得
	std::deque<std::unique_ptr<Sound>>& players = audioMap_[channelID];

	// 取得した配列内に何も入っていなかったら早期リターン
	if (players.empty()) { return; }

	// 配列内の全ての効果音の音量を調節
	for (auto& p : players) {
		p->SetVolume(volume);
	}
}
