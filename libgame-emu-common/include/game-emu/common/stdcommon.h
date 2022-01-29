#pragma once

// The Windows header define min and max macros for some reason??? Really stupid choice as they collide with std::min and std::max.
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

#include <game-emu/common/util.h>
#include <game-emu/common/symbolhelper.h>
