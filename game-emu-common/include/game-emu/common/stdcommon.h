#pragma once

// The Windows headers define min and max macros for some reason??? Really stupid choice as they collide with std::min and std::max.
#define NOMINMAX

#include <cstdlib>
#include <cstdio>
#include <iostream>
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
#include <bit>
#include <functional>

#include <game-emu/common/util.h>
#include <game-emu/common/endianutils.h>
#include <game-emu/common/symbolhelper.h>
