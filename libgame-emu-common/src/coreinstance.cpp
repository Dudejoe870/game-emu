#include <game-emu/common/coreinstance.h>

#include <game-emu/common/core.h>

namespace GameEmu::Common
{
	CoreInstance::CoreInstance(Core* core, const std::unordered_map<std::string, PropertyValue>& properties)
	{
		this->core = core;

		this->properties.insert(properties.begin(), properties.end());
	}

	int CoreInstance::addInstance(Core* core, std::unordered_map<std::string, PropertyValue> properties)
	{
		instances.push_back(core->createNewInstance(properties));
		return (int)instances.size() - 1;
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
