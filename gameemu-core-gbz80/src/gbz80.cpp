#include <game-emu/cores/processor/gbz80.h>

namespace GameEmu::Cores::Processor::GBZ80
{
	Instance::Instance(Common::Core* core, const std::unordered_map<std::string, Common::PropertyValue>& properties)
		: Common::CoreInstance(core, properties)
	{
	}

	Common::CoreInstance::ReturnStatus Instance::Step()
	{
		return ReturnStatus::Success;
	}

	Core::Core(Common::CoreLoader* loader)
		: Common::Core(loader)
	{
	}

	std::string Core::getName()
	{
		return "gameemu-core-gbz80";
	}

	Common::Core::Type Core::getType()
	{
		return Common::Core::Type::Processor;
	}

	Common::CoreInstance* Core::createNewInstance(std::unordered_map<std::string, Common::PropertyValue> properties)
	{
		return new Instance(this, properties);
	}
}
