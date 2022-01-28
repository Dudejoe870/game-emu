#pragma once

#include <game-emu/common/stdcommon.h>

namespace GameEmu::Common
{
	typedef std::variant<int, long long, char, float, double, bool, std::string> PropertyValue;
}
