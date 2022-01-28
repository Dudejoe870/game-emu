#include <game-emu/common/coreinstance.h>

#include <game-emu/common/core.h>

namespace GameEmu::Common
{
	CoreInstance::CoreInstance(Core* core, const std::unordered_map<std::string, PropertyValue>& properties)
	{
		this->core = core;
		this->properties = properties;
	}

	CoreInstance::ReturnStatus CoreInstance::Step()
	{
		return CoreInstance::ReturnStatus::Error;
	}

	CoreInstance::ReturnStatus CoreInstance::SystemInit()
	{
		return CoreInstance::ReturnStatus::Error;
	}
}
