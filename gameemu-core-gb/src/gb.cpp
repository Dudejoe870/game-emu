#include <game-emu/cores/system/gb.h>

namespace GameEmu::Cores::System::GB
{
	Instance::Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties)
		: Common::CoreInstance(core, runState, properties), mainMemoryMap(0xFFFF)
	{
		this->sm83 = AddInstance<Processor::SM83::Instance>(core->GetDependency("sm83"), { { "freq", static_cast<s64>(1050000) } }); // 1.05 MHz

		romBank0.resize(0x3FFF+1);
		bios.resize(0xFF+1);

		// Start with the BIOS mapped over the Cartridge ROM
		biosEntry = mainMemoryMap.Map(bios.data(), bios.data(), bios.size(), 0x0000);
		romBank0Entry = mainMemoryMap.Map(romBank0.data()+bios.size(), romBank0.data()+bios.size(), romBank0.size()-bios.size(), 0x0000 + bios.size());

		mainMemoryMap.Map(&state.ioState.boot, &state.ioState.boot, 1, 0xFF50, nullptr, 
			[&](u64 readValue, u64 writeValue, u64 valueToBeWritten, u64 address, u8 size)
			{
				if ((readValue & 0b1) == 0 && valueToBeWritten & 0b1)
				{
					runState.logger->LogInfo("GB Unmapping BIOS");

					// Unmap the BIOS
					mainMemoryMap.Unmap(biosEntry);

					// Remap the Cartridge ROM
					mainMemoryMap.Unmap(romBank0Entry);
					romBank0Entry = mainMemoryMap.Map(romBank0.data(), romBank0.data(), romBank0.size(), 0x0000);

					return static_cast<u64>(0b1);
				}
				return readValue;
			});
		runState.logger->LogInfo("GB initialized memory map.");

		sm83->AddAddressSpace(mainMemoryMap, "idmem"); // Add the Instruction / Data Memory Map to the SM83 CPU.
		runState.logger->LogInfo("GB added SM83 address space.");
	}

	Common::CoreInstance::ReturnStatus Instance::Init()
	{
		return ReturnStatus::Success;
	}

	Common::CoreState* Instance::GetCoreState()
	{
		return &state;
	}

	Core::Core(Common::CoreLoader* loader)
		: Common::Core(loader)
	{
	}

	std::string Core::GetName()
	{
		return "gb";
	}

	std::string Core::GetDescription()
	{
		return "A Gameboy emulation Core.";
	}

	std::unordered_map<std::string, Common::PropertyValue> Core::GetDefaultProperties()
	{
		return { { "romfile", "" } };
	}

	Common::Core::Type Core::GetType()
	{
		return Common::Core::Type::System;
	}

	std::shared_ptr<Common::CoreInstance> Core::CreateNewInstance(Common::RunState& runState, std::unordered_map<std::string, Common::PropertyValue> properties)
	{
		return std::make_shared<Instance>(this, runState, properties);
	}
}
