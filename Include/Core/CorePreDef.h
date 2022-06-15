#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <cstddef>
#include <utility>
#include <format>
#include <string>
#include <string_view>
#include <chrono>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <wrl/client.h>
#include <combaseapi.h>
#include <functional>
#include <vector>

#include <DirectXMath.h>
#include <optional>
#include <fstream>
