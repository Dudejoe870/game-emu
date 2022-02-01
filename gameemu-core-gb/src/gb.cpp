#include <game-emu/cores/system/gb.h>

namespace GameEmu::Cores::System::GB
{
	Instance::Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties)
		: Common::CoreInstance(core, runState, properties)
	{
		this->gbCore = (Core*)core;

		this->z80 = addInstance(this->gbCore->getDependencies()[this->gbCore->z80]);
	}

	Common::CoreInstance::ReturnStatus Instance::Step()
	{
		return ReturnStatus::Success;
	}

	Common::CoreInstance::ReturnStatus Instance::SystemInit()
	{
		return ReturnStatus::Success;
	}

	Core::Core(Common::CoreLoader* loader)
		: Common::Core(loader)
	{
		this->z80 = -1;
	}

	void Core::LoadDependencies()
	{
		this->z80 = addDependency("gameemu-core-gbz80");
	}

	std::string Core::getName()
	{
		return "gameemu-core-gb";
	}

	std::string Core::getDescription()
	{
		return "A Gameboy emulation Core.";
	}

	std::unordered_map<std::string, Common::PropertyValue> Core::getDefaultProperties()
	{
		return { { "romfile", "" } };
	}

	Common::Core::Type Core::getType()
	{
		return Common::Core::Type::System;
	}

	std::unique_ptr<Common::CoreInstance> Core::createNewInstance(Common::RunState& runState, std::unordered_map<std::string, Common::PropertyValue> properties)
	{
		return std::make_unique<Instance>(this, runState, properties);
	}
}
