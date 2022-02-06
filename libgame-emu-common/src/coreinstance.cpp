#include <game-emu/common/coreinstance.h>

#include <game-emu/common/core.h>

namespace GameEmu::Common
{
	CoreInstance::CoreInstance(Core* core, RunState& runState,
		const std::unordered_map<std::string, PropertyValue>& properties) 
		: runState(runState)
	{
		this->core = core;
		this->paused = false;

		this->properties = core->getDefaultProperties();

		this->properties.insert(properties.begin(), properties.end());
	}

	CoreInstance::~CoreInstance()
	{
	}

	int CoreInstance::addInstance(Core* core, const std::unordered_map<std::string, PropertyValue>& properties)
	{
		instances.push_back(core->createNewInstance(runState, properties));
		return (int)instances.size() - 1;
	}

	CoreInstance::ReturnStatus CoreInstance::Step()
	{
		return ReturnStatus::Error;
	}

	CoreInstance::ReturnStatus CoreInstance::SystemInit()
	{
		return ReturnStatus::Error;
	}

	bool CoreInstance::isMultithreaded()
	{
		return false;
	}

	CoreState* CoreInstance::getCoreState()
	{
		return nullptr;
	}

	std::chrono::nanoseconds CoreInstance::getStepPeriod()
	{
		return std::chrono::nanoseconds::zero();
	}
}
