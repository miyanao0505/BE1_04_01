#pragma once
#include <cstdint>

/// <summary>
/// ロジック名
/// </summary>
enum class LogicName : uint8_t {
	WAIT_START,
	RUNNING,
	RESULT,
	RANKING,
};