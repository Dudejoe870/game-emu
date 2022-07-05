#include <game-emu/common/core.h>

#include <game-emu/common/coreloader.h>
#include <game-emu/common/coreinstance.h>

#include <dynalo/dynalo.hpp>

namespace GameEmu::Common
{
	Core::Core(GameEmu::Common::CoreLoader& loader)
		: loader(loader)
	{
	}

	Core::~Core()
	{
	}

	Core* Core::GetDependency(const std::string& coreName)
	{
		return loader.GetLoadedCore(coreName);
	}

	const std::string Core::GetName() const
	{
		return "unknown";
	}

	const std::string Core::GetDescription() const
	{
		return "";
	}

	std::unordered_map<std::string, PropertyValue> Core::GetDefaultProperties() const
	{
		return { };
	}

	Core::Type Core::GetType() const
	{
		return Core::Type::Processor;
	}
}
