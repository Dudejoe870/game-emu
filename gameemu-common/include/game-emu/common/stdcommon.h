#pragma once

#ifdef _WIN32
// The Windows headers define min and max macros for some reason??? Really stupid choice as they collide with std::min and std::max.
#define NOMINMAX
#endif

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <variant>
#include <fmt/core.h>
#include <functional>
#include <array>
#include <algorithm>
#include <deque>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;
