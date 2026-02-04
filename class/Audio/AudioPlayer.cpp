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
	float playVolume = *masterVolume * volume_ * volumeMultipler_;

	// 音量設定
	audio_.SetVolume(playVolume);

	// 減衰計算
	CheckDistanceUpdate();
}

void AudioPlayer::Update() {
	// 減衰に関する更新
	CheckDistanceUpdate();
}

void AudioPlayer::Stop() {
	// 音の停止
	audio_.Stop();
}

void AudioPlayer::SetVolume(float volume)
{
	// 音量設定
	volume_ = volume;
	float playVolume = *masterVolume * volume_ * volumeMultipler_;
	audio_.SetVolume(playVolume);
}

void AudioPlayer::SetVolumeMultiply(float multipler)
{
	// 音量倍率設定
	volumeMultipler_ = multipler;
	SetVolume(volume_);
}

void AudioPlayer::CheckDistanceUpdate()
{
	// リスナーが何も無ければ早期リターン
	if (listener_ == nullptr) { return; }

	// リスナーとの距離を求める
	LWP::Math::Vector3 distanceVec = listener_->GetWorldPosition() - emitPos_;
	float distance = distanceVec.Length();

	// 最小距離以下なら最大音量で鳴らす
	if (distance <= minDistance_) {
		SetVolumeMultiply(maxVolumeMultiply_);
		return;
	}

	// 最小距離以下なら最大音量で鳴らす
	if (distance >= maxDistance_) {
		SetVolumeMultiply(minVolumeMultiply_);
		return;
	}

	// 線形補間で音量を求める
	float t = (distance - minDistance_) / (maxDistance_ - minDistance_);
	float V = 1.0f - t;

	// 二乗したもので音量設定
	SetVolumeMultiply(V * V);
}
