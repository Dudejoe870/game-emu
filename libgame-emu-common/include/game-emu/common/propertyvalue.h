#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/symbolhelper.h>

namespace GameEmu::Common
{
	using PropertyValue = std::variant<int, long long, char, short, float, double, bool, std::string>;
	namespace Util
	{
		enum class PropertyValueType
		{
			Integer32,
			Integer64,
			Integer8,
			Integer16,
			Float,
			Double,
			Bool,
			String,
			Unknown
		};

		LIBGAMEEMU_COMMON_DLL_EXPORT PropertyValueType getPropertyValueType(const PropertyValue& value);
		LIBGAMEEMU_COMMON_DLL_EXPORT std::string getPropertyValueTypeDisplayName(PropertyValueType type);
	}
}
