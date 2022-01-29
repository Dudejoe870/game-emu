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

		std::vector<std::unique_ptr<CoreInstance>> instances;

		/*
		 Adds a new core instance and returns the index at which it is stored in the instances vector.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT int addInstance(Core* core, std::unordered_map<std::string, PropertyValue> properties = {});
	public:
		enum class ReturnStatus
		{
			Success,
			Error
		};

		inline const std::vector<std::unique_ptr<CoreInstance>>& getInstances()
		{
			return instances;
		}

		inline Core* getCore()
		{
			return core;
		}

		// KeyValue map Properties for this particular Core instance, value can be an int, string, float, or bool of any size.
		std::unordered_map<std::string, PropertyValue> properties;

		LIBGAMEEMU_COMMON_DLL_EXPORT CoreInstance(Core* core, const std::unordered_map<std::string, PropertyValue>& properties);

		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus Step();
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus SystemInit();
	};
}
