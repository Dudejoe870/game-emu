#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/propertyvalue.h>

namespace GameEmu::Common
{
	class CoreLoader;

	class Core
	{
	protected:
		CoreLoader* loader;
	public:
		enum class Type
		{
			Processor, // A Processor like a CPU or some other type of linear instruction-based processor.
			GraphicsRasterizer, // A Graphics Rasterizer can accept instructions just like a regular processor, but also has access to rendering with the host GPU.
			System // A System loads other cores together and invokes them.
		};

		// Properties common to all core instances.
		std::unordered_map<std::string, PropertyValue> globalProperties;

		LIBGAMEEMU_COMMON_DLL_EXPORT Core(CoreLoader* loader);

		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::string getName();
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual Type getType();
	};
}
