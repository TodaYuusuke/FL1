#pragma once
#include "Sound.h"
#include <deque>

/// <summary>
/// 音再生管理クラス
/// </summary>
class AudioPlayer final : public LWP::Utility::ISingleton<AudioPlayer> {
	friend class LWP::Utility::ISingleton<AudioPlayer>;

public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AudioPlayer();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AudioPlayer();

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
	/// <param name="isLoop">(任意) ループを行うか</param>
	int32_t PlayAudio(const std::string& filePath, float volume, int channelID, const bool isLoop = false);

	/// <summary>
	/// 再生関数
	/// </summary>
	/// <param name="filePath">オーディオまでのファイルパス</param>
	/// <param name="volume">音量</param>
	/// <param name="channelID">再生するチャンネルID</param>
	/// <param name="pos">音の発生座標</param>
	/// <param name="isLoop">ループを行うか</param>
	int32_t PlayAudio(const std::string& filePath, float volume, int channelID, const LWP::Math::Vector3& pos, const bool isLoop = false);

	/// <summary>
	/// 引数で指定された数からランダムな数を求め、末尾に追加したパスで再生する関数
	/// </summary>
	/// <param name="filePath">オーディオまでのファイルパス</param>
	/// <param name="maxRandomCount">ランダム最大数</param>
	/// <param name="volume">音量</param>
	/// <param name="channelID">再生するチャンネルID</param>
	int32_t PlayRandomAudio(const std::string& filePath, int maxRandomCount, float volume, int channelID);

	/// <summary>
	/// 再生関数
	/// </summary>
	/// <param name="filePath">オーディオまでのファイルパス</param>
	/// <param name="volume">音量</param>
	/// <param name="maxRandomCount">ランダム最大数</param>
	/// <param name="channelID">再生するチャンネルID</param>
	/// <param name="pos">音の発生座標</param>
	int32_t PlayRandomAudio(const std::string& filePath, int maxRandomCount, float volume, int channelID, const LWP::Math::Vector3& pos);

public: // アクセッサ等

	/// <summary>
	/// リスナーセッター
	/// </summary>
	/// <param name="listener">リスナー</param>
	void SetListener(const LWP::Object::TransformQuat* listener) { listener_ = listener; }

	/// <summary>
	/// オーディオプレイヤーのゲッター
	/// </summary>
	/// <param name="id">生成時に渡される固有ID</param>
	/// <returns>実体</returns>
	Sound* GetAudioPlayer(const uint32_t id);

	/// <summary>
	/// 指定されたチャンネルの音量を調節する
	/// </summary>
	/// <param name="volume">音量</param>
	/// <param name="channelID">チャンネル番号</param>
	void SetChannelVolume(const float volume, const int channelID);

	/// <summary>
	/// 音全体の音量を調節する
	/// </summary>
	/// <param name="masterVolume">効果音全体の音量</param>
	void SetMasterVlume(const float masterVolume) { masterVolume_ = masterVolume; }

private: // メンバ変数

	// 効果音全体の音量
	float masterVolume_ = 1.0f;

	// リスナー座標
	const LWP::Object::TransformQuat* listener_ = nullptr;

	// 効果音マップ
	std::map<int, std::deque<std::unique_ptr<Sound>>> audioMap_{};

	// 固有ID配列
	std::unordered_map<uint32_t, Sound*> idMap_{};
	// 次のID
	uint32_t nextID_ = 1;
};

