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

	int Core::addDependency(std::string coreName)
	{
		dependencies.push_back(loader->getLoadedCore(coreName));
		return (int)dependencies.size() - 1;
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

	std::unordered_map<std::string, PropertyValue> Core::getDefaultProperties()
	{
		return { };
	}

	Core::Type Core::getType()
	{
		return Core::Type::Processor;
	}

	std::unique_ptr<CoreInstance> Core::createNewInstance(RunState& runState, std::unordered_map<std::string, PropertyValue> properties)
	{
		return std::make_unique<CoreInstance>(this, runState, properties);
	}
}
