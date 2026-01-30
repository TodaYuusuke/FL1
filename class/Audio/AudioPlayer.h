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
	/// 再生状態ゲッター
	/// </summary>
	/// <returns>再生されているかどうか</returns>
	bool GetIsPlaying() { return audio_.GetIsPlaying(); }

private: // メンバ変数

	// 音リソース本体
	LWP::Resource::Audio audio_;

	// 大元の音量
	const float* masterVolume = nullptr;

	// 音量
	float volume_ = 1.0f;

	// ループフラグ
	bool isLoop_ = false;

};

