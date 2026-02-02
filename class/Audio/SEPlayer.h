#pragma once
#include "AudioPlayer.h"
#include "AudioConfig.h"

/// <summary>
/// 効果音再生管理クラス
/// </summary>
class SEPlayer final : public LWP::Utility::ISingleton<SEPlayer> {
	friend class LWP::Utility::ISingleton<SEPlayer>;

public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SEPlayer();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SEPlayer();

public: // メンバ関数

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 再生関数
	/// </summary>
	/// <param name="filePath">オーディオまでのファイルパス</param>
	/// <param name="volume">音量</param>
	/// <param name="channelID">再生するチャンネルID</param>
	void PlaySE(const std::string& filePath, float volume, int channelID);

	/// <summary>
	/// 引数で指定された数からランダムな数を求め、末尾に追加したパスで再生する関数
	/// </summary>
	/// <param name="filePath">オーディオまでのファイルパス</param>
	/// <param name="maxRandomCount">ランダム最大数</param>
	/// <param name="volume">音量</param>
	/// <param name="channelID">再生するチャンネルID</param>
	void PlayRandomSE(const std::string& filePath, int maxRandomCount, float volume, int channelID);

public: // アクセッサ等

	/// <summary>
	/// 指定されたチャンネルの音量を調節する
	/// </summary>
	/// <param name="volume">音量</param>
	/// <param name="channelID">チャンネル番号</param>
	void SetVolume(const float volume, const int channelID);

	/// <summary>
	/// 効果音全体の音量を調節する
	/// </summary>
	/// <param name="masterVolume">効果音全体の音量</param>
	void SetMasterVlume(const float masterVolume) { masterVolume_ = masterVolume; }

private: // メンバ変数

	// 効果音全体の音量
	float masterVolume_ = 1.0f;

	// 効果音マップ
	std::map<int, std::vector<AudioPlayer>> seMap_{};

};

