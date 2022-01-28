#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/propertyvalue.h>

namespace GameEmu::Common
{
	class Core;

	class CoreInstance
	{
	protected:
		Core* core;
	public:
		enum class ReturnStatus
		{
			Success,
			Error
		};

		std::unordered_map<std::string, PropertyValue> properties;

		LIBGAMEEMU_COMMON_DLL_EXPORT CoreInstance(Core* core, const std::unordered_map<std::string, PropertyValue>& properties);

		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus Step();
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus SystemInit();
	};
}
