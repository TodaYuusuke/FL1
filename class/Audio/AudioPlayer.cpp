#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(const std::string& filePath, float volume, bool isLoop, const float* masterV)
{
	// 音のロード
	audio_.LoadShortPath(filePath);

	// 各種値の取得
	volume_ = volume;
	isLoop_ = isLoop;

	// マスターボリュームの取得
	masterVolume = masterV;
}

void AudioPlayer::Play()
{
	// ループするか否かで処理を分岐
	if (isLoop_) {
		audio_.PlayLoop();
	}
	else {
		audio_.Play();
	}

	// 再生音量を求める
	float playVolume = *masterVolume * volume_;

	// 音量設定
	audio_.SetVolume(playVolume);
}

void AudioPlayer::Stop() {
	// 音の停止
	audio_.Stop();
}

void AudioPlayer::SetVolume(float volume)
{
	// 音量設定
	volume_ = volume;
	float playVolume = *masterVolume * volume_;
	audio_.SetVolume(playVolume);
}