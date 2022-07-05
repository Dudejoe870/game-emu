#include <game-emu/cores/system/gb.h>

namespace GameEmu::Cores::System::GB
{
	Instance::Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& propertyOverrides)
		: Common::SystemInstance(core, runState, propertyOverrides)
	{
		this->mainMemoryMap = std::make_shared<Common::BinaryTreeMemoryMap>(0xFFFF);
		
		this->sm83 = this->AddChild<Processor::SM83::Instance>(
			core->GetDependency("sm83"), { { "freq", static_cast<s64>(1050000) } }); // 1.05 MHz

		romBank0.resize(0x3FFF+1);
		bios.resize(0xFF+1);

		// Start with the BIOS mapped over the Cartridge ROM
		biosEntry = mainMemoryMap->Map(bios.data(), bios.data(), bios.size(), 0x0000);
		romBank0Entry = mainMemoryMap->Map(romBank0.data()+bios.size(), romBank0.data()+bios.size(), romBank0.size()-bios.size(), 0x0000 + bios.size());

		mainMemoryMap->Map(&state.ioState.boot, &state.ioState.boot, 1, 0xFF50, nullptr,
			[this](u64 readValue, u64 writeValue, u64 valueToBeWritten, u64 address, u8 size)
			{
				if ((readValue & 0b1) == 0 && valueToBeWritten & 0b1)
				{
					logger->LogInfo("Unmapping BIOS");

					// Unmap the BIOS
					mainMemoryMap->Unmap(biosEntry);

					// Remap the Cartridge ROM
					mainMemoryMap->Unmap(romBank0Entry);
					romBank0Entry = mainMemoryMap->Map(romBank0.data(), romBank0.data(), romBank0.size(), 0x0000);

					return static_cast<u64>(0b1);
				}
				return readValue;
			});
		logger->LogInfo("Initialized memory map.");

		sm83->AddMemoryMap(mainMemoryMap, "idmem"); // Add the Instruction / Data Memory Map to the SM83 CPU.
		logger->LogInfo("Added SM83 address space.");

		// Load the BIOS into memory.
		if (this->properties.contains("bios") &&
			Common::Util::GetPropertyValueType(this->properties.at("bios")) == Common::Util::PropertyValueType::String)
		{
			std::string biosPath = std::get<std::string>(this->properties.at("bios"));
			
			logger->LogInfo("Loading BIOS from " + biosPath);
			std::ifstream biosFile(biosPath, std::ios::binary);
			if (biosFile.is_open())
			{
				biosFile.read(reinterpret_cast<char*>(bios.data()), bios.size());
				biosFile.close();
			}
			else logger->LogError("Failed to load BIOS from " + biosPath);
		}
		else logger->LogError("No bios specified");
	}

	Common::CoreInstance::ReturnStatus Instance::Init()
	{
		return ReturnStatus::Success;
	}

	const Common::CoreState& Instance::GetCoreState() const
	{
		return state;
	}

	Core::Core(Common::CoreLoader& loader)
		: Common::Core(loader)
	{
	}

	const std::string Core::GetName() const
	{
		return "gb";
	}
	
	const std::string Core::GetDescription() const
	{
		return "A Gameboy emulation Core.";
	}

	std::unordered_map<std::string, Common::PropertyValue> Core::GetDefaultProperties() const
	{
		return { { "romfile", "" }, { "bios", "" } };
	}

	Common::Core::Type Core::GetType() const
	{
		return Common::Core::Type::System;
	}

	std::shared_ptr<Common::CoreInstance> Core::CreateNewInstance(
		Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& propertyOverrides)
	{
		return std::make_shared<Instance>(this, runState, propertyOverrides);
	}
}
