#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/core.h>
#include <game-emu/common/coreinstance.h>
#include <game-emu/common/coreloader.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/cores/processor/sm83.h>

#include <game-emu/common/physicalmemorymap.h>

namespace GameEmu::Cores::System::GB
{
	class Core;

	class Instance : public Common::CoreInstance
	{
	private:
		Processor::SM83::Instance* sm83;

		std::vector<u8> bios;
		Common::PhysicalMemoryMap::Entry* biosEntry;

		std::vector<u8> romBank0;
		Common::PhysicalMemoryMap::Entry* romBank0Entry;

		Common::BinaryTreeMemoryMap mainMemoryMap;
	public:
		Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties);

		ReturnStatus Init();
	};

	class Core : public Common::Core
	{
	public:
		Core(Common::CoreLoader* loader);

		std::string getName();
		Common::Core::Type getType();
		std::string getDescription();
		std::unordered_map<std::string, Common::PropertyValue> getDefaultProperties();

		std::shared_ptr<Common::CoreInstance> createNewInstance(Common::RunState& runState, std::unordered_map<std::string, Common::PropertyValue> properties = {});
	};
}
