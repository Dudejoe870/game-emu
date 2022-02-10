#include <game-emu/cores/system/gb.h>

namespace GameEmu::Cores::System::GB
{
	Instance::Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties)
		: Common::CoreInstance(core, runState, properties), mainMemoryMap(0xFFFF)
	{
		this->sm83 = addInstance<Processor::SM83::Instance>(core->getDependency("sm83"), { { "freq", static_cast<s64>(1050000) } }); // 1.05 MHz

		romBank0.resize(0x3FFF+1);
		bios.resize(0xFF+1);

		// Start with the BIOS mapped over the Cartridge ROM
		romBank0Entry = mainMemoryMap.Map(romBank0.data()+bios.size(), romBank0.data()+bios.size(), romBank0.size(), 0x0000+bios.size());
		biosEntry = mainMemoryMap.Map(bios.data(), bios.data(), bios.size(), 0x0000);

		mainMemoryMap.Update();

		sm83->addAddressSpace(mainMemoryMap, "idmem"); // Add the Instruction / Data Memory Map to the SM83 CPU.
	}

	Common::CoreInstance::ReturnStatus Instance::Init()
	{
		return ReturnStatus::Success;
	}

	Core::Core(Common::CoreLoader* loader)
		: Common::Core(loader)
	{
	}

	std::string Core::getName()
	{
		return "gb";
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

	std::shared_ptr<Common::CoreInstance> Core::createNewInstance(Common::RunState& runState, std::unordered_map<std::string, Common::PropertyValue> properties)
	{
		return std::make_shared<Instance>(this, runState, properties);
	}
}
