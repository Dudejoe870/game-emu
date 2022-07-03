#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/symbolhelper.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/runstate.h>

#include <game-emu/common/instructiondecoder.h>

namespace GameEmu::Common
{
	class CoreLoader;
	class CoreInstance;

	class Core
	{
	private:
		CoreLoader* loader;
	public:
		enum class Type
		{
			Processor, // A Processor of any kind of data, like a CPU.
			GraphicsRasterizer, // A Graphics Rasterizer can step just like a regular processor, but also has access to rendering with the host GPU.
			System // A System loads other cores together and glues them together as a full system.
		};

		/*
		 Get a pointer to a dependency.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT Core* GetDependency(std::string coreName);

		/*
		 Initially loads the Core.
		 Note: There is no guaranty of load order.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT Core(CoreLoader* loader);

		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ~Core();

		/*
		 Returns the Cores name.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::string GetName();

		/*
		 Returns a short description of the Core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::string GetDescription();

		/*
		 Returns the default property values for this Core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::unordered_map<std::string, PropertyValue> GetDefaultProperties();

		/*
		 Returns the Core type.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual Type GetType();

		/*
		 Creates a new Core instance.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::shared_ptr<CoreInstance> CreateNewInstance(
			RunState& runState, std::unordered_map<std::string, PropertyValue> properties = {});
	};
}
