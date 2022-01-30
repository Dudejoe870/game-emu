#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/core.h>
#include <game-emu/common/coreinstance.h>
#include <game-emu/common/coreloader.h>

#include <game-emu/common/propertyvalue.h>

namespace GameEmu::Cores::Processor::GBZ80
{
	class Instance : public Common::CoreInstance
	{
	public:
		Instance(Common::Core* core, const std::unordered_map<std::string, Common::PropertyValue>& properties);

		ReturnStatus Step();
	};

	class Core : public Common::Core
	{
	public:
		Core(Common::CoreLoader* loader);
		
		std::string getName();
		std::string getDescription();
		Common::Core::Type getType();
		std::unique_ptr<Common::CoreInstance> createNewInstance(std::unordered_map<std::string, Common::PropertyValue> properties = {});
	};
}
