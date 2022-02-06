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
	protected:
		CoreLoader* loader;

		std::vector<Core*> dependencies;

		/*
		 Adds a dependency and returns the index in which it is stored in the dependencies vector.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT int addDependency(std::string coreName);
	public:
		enum class Type
		{
			Processor, // A Processor of any kind of data, like a CPU.
			GraphicsRasterizer, // A Graphics Rasterizer can step just like a regular processor, but also has access to rendering with the host GPU.
			System // A System loads other cores together and glues them together as a full system.
		};

		inline const std::vector<Core*>& getDependencies()
		{
			return dependencies;
		}

		/*
		 Initially loads the Core.
		 Note: There is no guaranty of load order.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT Core(CoreLoader* loader);

		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ~Core();

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
		 Returns a short description of the Core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::string getDescription();

		/*
		 Returns the default property values for this Core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::unordered_map<std::string, PropertyValue> getDefaultProperties();

		/*
		 Returns the Core type.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual Type getType();

		/*
		 Creates a new Core instance.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::unique_ptr<CoreInstance> createNewInstance(RunState& runState, std::unordered_map<std::string, PropertyValue> properties = {});
	};
}
