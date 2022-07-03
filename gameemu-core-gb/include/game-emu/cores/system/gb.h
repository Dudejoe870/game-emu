#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/core.h>
#include <game-emu/common/coreinstance.h>
#include <game-emu/common/coreloader.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/cores/processor/sm83.h>

#include <game-emu/common/corestate.h>
#include <game-emu/common/register.h>

#include <game-emu/common/physicalmemorymap.h>

namespace GameEmu::Cores::System::GB
{
	class Core;

	class State : public Common::CoreState
	{
	public:
		struct IOState
		{
			u8 boot; // 0xFF50

			IOState()
			{
				std::memset(this, 0, sizeof(IOState));
			}
		};

		IOState ioState;

		Common::Register<u8, std::endian::native> boot;

		State() 
			: boot(this, ioState.boot, ioState.boot, "BOOT", true, "{name} (0xFF50): 0x{u8:02x}")
		{
		}
	};

	class Instance : public Common::CoreInstance
	{
	private:
		Processor::SM83::Instance* sm83;

		State state;

		std::vector<u8> bios;
		std::shared_ptr<Common::PhysicalMemoryMap::Entry> biosEntry;

		std::vector<u8> romBank0;
		std::shared_ptr<Common::PhysicalMemoryMap::Entry> romBank0Entry;

		Common::BinaryTreeMemoryMap mainMemoryMap;
	public:
		Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties);

		ReturnStatus Init();

		Common::CoreState* GetCoreState();
	};

	class Core : public Common::Core
	{
	public:
		Core(Common::CoreLoader* loader);

		std::string GetName();
		Common::Core::Type GetType();
		std::string GetDescription();
		std::unordered_map<std::string, Common::PropertyValue> GetDefaultProperties();

		std::shared_ptr<Common::CoreInstance> CreateNewInstance(
			Common::RunState& runState, std::unordered_map<std::string, Common::PropertyValue> properties = {});
	};
}
