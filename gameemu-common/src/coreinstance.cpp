#include <game-emu/common/coreinstance.h>

#include <game-emu/common/core.h>

namespace GameEmu::Common
{
	CoreInstance::CoreInstance(Core* core, RunState& runState, const std::unordered_map<std::string, PropertyValue>& propertyOverrides)
		: core(core), runState(runState)
	{
		this->paused = false;

		this->properties = core->GetDefaultProperties();

		for (const auto& pair : propertyOverrides)
			this->properties[pair.first] = pair.second;
	}

	CoreInstance::~CoreInstance()
	{
	}

	CoreInstance::ReturnStatus CoreInstance::Step()
	{
		return ReturnStatus::Success;
	}

	CoreInstance::ReturnStatus CoreInstance::Init()
	{
		return ReturnStatus::Success;
	}

	std::chrono::nanoseconds CoreInstance::GetStepPeriod() const
	{
		return std::chrono::nanoseconds::zero();
	}

	u64 SystemInstance::AddChildImpl(Core* dependency, const std::unordered_map<std::string, PropertyValue>& propertyOverrides)
	{
		children.push_back(dependency->CreateNewInstance(runState, propertyOverrides));
		return children.size() - 1;
	}

	bool SystemInstance::IsMultithreaded() const
	{
		return false;
	}	
}
