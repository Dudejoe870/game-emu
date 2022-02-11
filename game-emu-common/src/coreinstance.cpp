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

	u64 CoreInstance::addInstanceImpl(Core* dependency, const std::unordered_map<std::string, PropertyValue>& properties)
	{
		instances.push_back(dependency->createNewInstance(runState, properties));
		return instances.size() - 1;
	}

	CoreInstance::ReturnStatus CoreInstance::Step()
	{
		return ReturnStatus::Error;
	}

	CoreInstance::ReturnStatus CoreInstance::Init()
	{
		return ReturnStatus::Success;
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
