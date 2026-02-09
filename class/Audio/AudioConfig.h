#pragma once

/// <summary>
/// オーディオ系の設定フォルダ
/// </summary>
namespace LWP::AudioConfig {

	/// <summary>
	/// 音チャンネルの列挙子
	/// </summary>
	enum AudioChannels
	{
		Player,		// プレイヤー
		Enemy,		// 敵
		Enviroment,	// 環境
		Other,		// その他
		BGM,		// BGM
		Count		// カウント用
	};

	/// <summary>
	/// 音読み込み時のデフォルトパス
	/// </summary>
	inline std::array<std::string, AudioChannels::Count> AudioPath = {
				"SE/",
				"SE/",
				"SE/",
				"SE/",
				"BGM/"
	};
}