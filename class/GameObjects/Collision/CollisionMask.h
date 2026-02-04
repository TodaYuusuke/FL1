#pragma once
#include <Adapter.h>

/// <summary>
/// 当たり判定のマスク
/// </summary>
namespace GameMask {
	// 自機
	inline int player = ColMask0;
	// 敵
	inline int enemy = ColMask1;
	// 攻撃
	inline int attack = ColMask2;
	// 構造物
	inline int prop = ColMask3;
};