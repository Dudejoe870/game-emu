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

		s32 z80;
	public:
		Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties);

		ReturnStatus SystemInit();
	};

	class Core : public Common::Core
	{
	public:
		s32 z80;

		Core(Common::CoreLoader* loader);

		void LoadDependencies();

		std::string getName();
		Common::Core::Type getType();
		std::string getDescription();
		std::unordered_map<std::string, Common::PropertyValue> getDefaultProperties();

		std::unique_ptr<Common::CoreInstance> createNewInstance(Common::RunState& runState, std::unordered_map<std::string, Common::PropertyValue> properties = {});
	};
}
