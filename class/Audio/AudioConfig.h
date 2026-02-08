#pragma once

/// <summary>
/// オーディオ系の設定フォルダ
/// </summary>
namespace LWP::AudioConfig {

	/// <summary>
	/// 効果音チャンネルの列挙子
	/// </summary>
	enum SEChannels
	{
		Player,		// プレイヤー
		Enemy,		// 敵
		Enviroment,	// 環境
		Other,		// その他
		BGM,		// BGM
		Count		// カウント用
	};

}