#include <game-emu/cores/system/gb.h>

namespace GameEmu::Cores::System::GB
{
	Instance::Instance(Common::Core* core, const std::unordered_map<std::string, Common::PropertyValue>& properties)
		: Common::CoreInstance(core, properties)
	{
		this->gbCore = (Core*)core;
		
		this->properties["romfile"] = "";

		this->z80 = (Processor::GBZ80::Instance*)this->gbCore->z80->createNewInstance();
	}

	Instance::~Instance()
	{
		if (z80) delete z80;
	}

	Common::CoreInstance::ReturnStatus Instance::Step()
	{
		this->z80->Step();
		return ReturnStatus::Success;
	}

	Common::CoreInstance::ReturnStatus Instance::SystemInit()
	{
		return ReturnStatus::Success;
	}

	Core::Core(Common::CoreLoader* loader)
		: Common::Core(loader)
	{
	}

	void Core::LoadDependencies()
	{
		this->z80 = (Processor::GBZ80::Core*)loader->getLoadedCore("gameemu-core-gbz80");
	}

	std::string Core::getName()
	{
		return "gameemu-core-gb";
	}

	std::string Core::getDescription()
	{
		return "A Gameboy emulation Core.";
	}

	Common::Core::Type Core::getType()
	{
		return Common::Core::Type::System;
	}

	Common::CoreInstance* Core::createNewInstance(std::unordered_map<std::string, Common::PropertyValue> properties)
	{
		return new Instance(this, properties);
	}
}
