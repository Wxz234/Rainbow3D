#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <wrl/client.h>
#include <memory>
#include <cstdint>

namespace Rainbow3D {
	using int8   = std::int8_t;
	using int16  = std::int16_t;
	using int32  = std::int32_t;
	using int64  = std::int64_t;
	using uint8  = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;
}
#define RAINBOW_FRAME_COUNT 3


