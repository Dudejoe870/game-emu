#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/core.h>
#include <game-emu/common/coreinstance.h>
#include <game-emu/common/coreloader.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/cores/processor/gbz80.h>

namespace GameEmu::Cores::System::GB
{
	class Core;

	class Instance : public Common::CoreInstance
	{
	private:
		Core* gbCore;

		Processor::GBZ80::Instance* z80;
	public:
		Instance(Common::Core* core, const std::unordered_map<std::string, Common::PropertyValue>& properties);
		~Instance();

		ReturnStatus Step();
		ReturnStatus SystemInit();
	};

	class Core : public Common::Core
	{
	public:
		Processor::GBZ80::Core* z80;

		Core(Common::CoreLoader* loader);

		void LoadDependencies();

		std::string getName();
		Common::Core::Type getType();
		std::string getDescription();

		Common::CoreInstance* createNewInstance(std::unordered_map<std::string, Common::PropertyValue> properties = {});
	};
}
