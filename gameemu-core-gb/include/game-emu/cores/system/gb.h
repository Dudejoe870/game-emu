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

		int z80;
	public:
		Instance(Common::Core* core, const std::unordered_map<std::string, Common::PropertyValue>& properties);

		ReturnStatus Step();
		ReturnStatus SystemInit();
	};

	class Core : public Common::Core
	{
	public:
		int z80;

		Core(Common::CoreLoader* loader);

		void LoadDependencies();

		std::string getName();
		Common::Core::Type getType();
		std::string getDescription();

		std::unique_ptr<Common::CoreInstance> createNewInstance(std::unordered_map<std::string, Common::PropertyValue> properties = {});
	};
}
