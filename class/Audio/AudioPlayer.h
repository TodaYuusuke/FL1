#pragma once
#include <Adapter.h>

/// <summary>
/// １つの音の再生、音量、ループを管理するクラス
/// </summary>
class AudioPlayer
{
public: // コンストラクタ

	// デフォルトコンストラクタ削除
	AudioPlayer() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="filePath">オーディオまでのファイルパス</param>
	/// <param name="volume">音量</param>
	/// <param name="isLoop">ループするか否か</param>
	AudioPlayer(const std::string& filePath, float volume, bool isLoop, const float* masterV);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AudioPlayer() = default;

public: // メンバ関数

	/// <summary>
	/// 再生
	/// </summary>
	void Play();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 停止
	/// </summary>
	void Stop();

public: // アクセッサ等

	/// <summary>
	/// 音量セッター
	/// </summary>
	/// <param name="volume">音量</param>
	void SetVolume(float volume);

	/// <summary>
	/// 音量倍率セッター
	/// </summary>
	/// <param name="multipler">音量倍率</param>
	void SetVolumeMultiply(float multipler);

	/// <summary>
	/// リスナーセッター
	/// </summary>
	/// <param name="listener">リスナー</param>
	void SetListener(const LWP::Object::TransformQuat* listener) { listener_ = listener; }

	/// <summary>
	/// 音の発生座標セッター
	/// </summary>
	/// <param name="pos">発生座標</param>
	void SetEmitPos(const LWP::Math::Vector3& pos) { emitPos_ = pos; }

	/// <summary>
	/// 最小音量倍率セッター
	/// </summary>
	/// <param name="minVolumeMulti">最小音量倍率</param>
	AudioPlayer& SetMinVolumeMultiply(const float minVolumeMulti) { minVolumeMultiply_ = minVolumeMulti; return *this; }
	/// <summary>
	/// 最大音量倍率セッター
	/// </summary>
	/// <param name="maxVolumeMulti">最大音量倍率</param>
	AudioPlayer& SetMaxVolumeMultiply(const float maxVolumeMulti) { maxVolumeMultiply_ = maxVolumeMulti; return *this; }

	/// <summary>
	/// 最小減衰距離セッター
	/// </summary>
	/// <param name="min">最小減衰距離</param>
	AudioPlayer& SetMinDistance(const float min) { minDistance_ = min; return *this; }

	/// <summary>
	/// 最大減衰距離セッター
	/// </summary>
	/// <param name="min">最大減衰距離</param>
	AudioPlayer& SetMaxDistance(const float max) { maxDistance_ = max; return *this; }

	/// <summary>
	/// 再生状態ゲッター
	/// </summary>
	/// <returns>再生されているかどうか</returns>
	bool GetIsPlaying() { return audio_.GetIsPlaying(); }

private: // 機能関数群

	/// <summary>
	/// 音量減衰チェック
	/// </summary>
	void CheckDistanceUpdate();

private: // メンバ変数

	// 音リソース本体
	LWP::Resource::Audio audio_;

	// 大元の音量
	const float* masterVolume = nullptr;

	// リスナー座標
	const LWP::Object::TransformQuat* listener_ = nullptr;
	// 音の発生座標
	LWP::Math::Vector3 emitPos_{};

	// 音量
	float volume_ = 1.0f;
	// 音量倍率
	float volumeMultipler_ = 1.0f;
	// 最小音量倍率
	float minVolumeMultiply_ = 0.35f;
	// 最大音量倍率
	float maxVolumeMultiply_ = 1.0f;

	// 距離減衰
	float minDistance_ = 30.0f;		// 最小距離
	float maxDistance_ = 200.0f;	// 最大距離

	// ループフラグ
	bool isLoop_ = false;

};

