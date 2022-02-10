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

	Core::~Core()
	{
	}

	Core* Core::getDependency(std::string coreName)
	{
		return loader->getLoadedCore(coreName);
	}

	std::string Core::getName()
	{
		return "unknown";
	}

	std::string Core::getDescription()
	{
		return "";
	}

	std::unordered_map<std::string, PropertyValue> Core::getDefaultProperties()
	{
		return { };
	}

	Core::Type Core::getType()
	{
		return Core::Type::Processor;
	}

	std::shared_ptr<CoreInstance> Core::createNewInstance(RunState& runState, std::unordered_map<std::string, PropertyValue> properties)
	{
		return std::make_shared<CoreInstance>(this, runState, properties);
	}
}
