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

	Core* Core::GetDependency(std::string coreName)
	{
		return loader->GetLoadedCore(coreName);
	}

	std::string Core::GetName()
	{
		return "unknown";
	}

	std::string Core::GetDescription()
	{
		return "";
	}

	std::unordered_map<std::string, PropertyValue> Core::GetDefaultProperties()
	{
		return { };
	}

	Core::Type Core::GetType()
	{
		return Core::Type::Processor;
	}

	std::shared_ptr<CoreInstance> Core::CreateNewInstance(RunState& runState, std::unordered_map<std::string, PropertyValue> properties)
	{
		return std::make_shared<CoreInstance>(this, runState, properties);
	}
}
