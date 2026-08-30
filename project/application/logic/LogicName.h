#pragma once
#include <cstdint>

/// <summary>
/// ロジック名
/// </summary>
enum class LogicName : uint8_t {
	WAIT_START,	// 待機状態
	RUNNING,	// 計測状態
	RESULT,		// 結果表示
	RANKING,	// ランキング表示
};