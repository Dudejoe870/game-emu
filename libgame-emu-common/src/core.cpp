#include <game-emu/common/core.h>

#include <game-emu/common/coreloader.h>
#include <game-emu/common/coreinstance.h>

#include <dynalo/dynalo.hpp>

namespace GameEmu::Common
{
	Core::Core(GameEmu::Common::CoreLoader* loader)
	{
		this->loader = loader;
	}

	void Core::LoadDependencies()
	{
	}

	std::string Core::getName()
	{
		return "gameemu-core-unknown";
	}

	std::string Core::getDescription()
	{
		return "";
	}

	Core::Type Core::getType()
	{
		return Core::Type::Processor;
	}

	CoreInstance* Core::createNewInstance(std::unordered_map<std::string, PropertyValue> properties)
	{
		return new CoreInstance(this, properties);
	}
}
