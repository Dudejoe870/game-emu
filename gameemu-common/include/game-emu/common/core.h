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
		CoreLoader& loader;
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
		LIBGAMEEMU_COMMON_DLL_EXPORT Core* GetDependency(const std::string& coreName);

		/*
		 Initially loads the Core.
		 Note: There is no guaranty of load order.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT Core(CoreLoader& loader);

		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ~Core();

		/*
		 Returns the Cores name.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual const std::string GetName() const;

		/*
		 Returns a short description of the Core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual const std::string GetDescription() const;

		/*
		 Returns the default property values for this Core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::unordered_map<std::string, PropertyValue> GetDefaultProperties() const;

		/*
		 Returns the Core type.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual Type GetType() const;

		/*
		 Creates a new Core instance.
		*/
		virtual std::shared_ptr<CoreInstance> CreateNewInstance(
			RunState& runState, const std::unordered_map<std::string, PropertyValue>& propertyOverrides = {}) = 0;
	};
}
