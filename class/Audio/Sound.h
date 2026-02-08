#pragma once
#include <Adapter.h>
#include "../Effect/Utility/DeltaTimer.h"
#include "AudioConfig.h"

/// <summary>
/// １つの音の再生、音量、ループを管理するクラス
/// </summary>
class Sound
{
public: // コンストラクタ

	// デフォルトコンストラクタ削除
	Sound() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="filePath">オーディオまでのファイルパス</param>
	/// <param name="channelID"></param>
	/// <param name="volume">音量</param>
	/// <param name="isLoop">ループするか否か</param>
	/// <param name="masterV">マスターボリュームのID</param>
	Sound(const std::string& filePath, int channelID, float volume, bool isLoop, const float* masterV);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sound() = default;

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

	/// <summary>
	/// 停止
	/// </summary>
	/// <param name="fadeTime">止まるまでにかかる時間</param>
	void Stop(const float fadeTime);

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
	/// 固有IDのゲッター
	/// </summary>
	/// <returns>固有ID</returns>
	uint32_t GetInstanceID() { return instanceID_; }
	/// <summary>
	/// 固有IDのセッター
	/// </summary>
	/// <param name="id">固有ID</param>
	void SetInstanceID(const uint32_t id) { instanceID_ = id; }

	/// <summary>
	/// 最小音量倍率セッター
	/// </summary>
	/// <param name="minVolumeMulti">最小音量倍率</param>
	Sound& SetMinVolumeMultiply(const float minVolumeMulti) { minVolumeMultiply_ = minVolumeMulti; return *this; }
	/// <summary>
	/// 最大音量倍率セッター
	/// </summary>
	/// <param name="maxVolumeMulti">最大音量倍率</param>
	Sound& SetMaxVolumeMultiply(const float maxVolumeMulti) { maxVolumeMultiply_ = maxVolumeMulti; return *this; }

	/// <summary>
	/// 最小減衰距離セッター
	/// </summary>
	/// <param name="min">最小減衰距離</param>
	Sound& SetMinDistance(const float min) { minDistance_ = min; return *this; }

	/// <summary>
	/// 最大減衰距離セッター
	/// </summary>
	/// <param name="min">最大減衰距離</param>
	Sound& SetMaxDistance(const float max) { maxDistance_ = max; return *this; }

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

	// 固有ID
	uint32_t instanceID_ = 0;

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
	float minVolumeMultiply_ = 0.1f;
	// 最大音量倍率
	float maxVolumeMultiply_ = 1.0f;

	// 距離減衰
	float minDistance_ = 20.0f;		// 最小距離
	float maxDistance_ = 150.0f;	// 最大距離

	// ループフラグ
	bool isLoop_ = false;

	// フェード演出用タイマー
	LWP::Utility::DeltaTimer fadeTimer_{};

};

