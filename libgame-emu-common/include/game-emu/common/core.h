#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/propertyvalue.h>

namespace GameEmu::Common
{
	class CoreLoader;
	class CoreInstance;

	class Core
	{
	protected:
		CoreLoader* loader;
	public:
		enum class Type
		{
			Processor, // A Processor of any kind of data, like a CPU.
			GraphicsRasterizer, // A Graphics Rasterizer can step just like a regular processor, but also has access to rendering with the host GPU.
			System // A System loads other cores together and invokes them.
		};

		// Properties common to all core instances.
		std::unordered_map<std::string, PropertyValue> globalProperties;

		/*
		 Initially loads the Core.
		 Note: There is no guaranty of load order.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT Core(CoreLoader* loader);

		/*
		 Loads all dependencies for other Cores.
		 Called after all Cores have already initially been added and loaded.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual void LoadDependencies();

		/*
		 Returns the Cores name.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::string getName();

		/*
		 Returns a short description of the Core. (Only relevant to System Cores)
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::string getDescription();

		/*
		 Returns the Core type.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual Type getType();

		/*
		 Creates a new Core instance. The caller is responsible for freeing the newly created instance.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual CoreInstance* createNewInstance(std::unordered_map<std::string, PropertyValue> properties = {});
	};
}
