#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/runstate.h>

#include <game-emu/common/corestate.h>

namespace GameEmu::Common
{
	class Core;

	class CoreInstance
	{
	protected:
		Core* core;

		std::vector<std::unique_ptr<CoreInstance>> instances;

		RunState& runState;

		/*
		 Adds a new core instance and returns the index at which it is stored in the instances vector.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT int addInstance(Core* core, const std::unordered_map<std::string, PropertyValue>& properties = {});
	public:
		std::atomic<bool> paused;

		enum class ReturnStatus
		{
			Success,
			Error
		};

		inline const std::vector<std::unique_ptr<CoreInstance>>& getInstances()
		{
			return instances;
		}

		inline Core* getCore()
		{
			return core;
		}

		// KeyValue map Properties for this particular Core instance, value can be an int, string, float, or bool of any size.
		std::unordered_map<std::string, PropertyValue> properties;
		
		LIBGAMEEMU_COMMON_DLL_EXPORT CoreInstance(Core* core, RunState& runState,
			const std::unordered_map<std::string, PropertyValue>& properties);

		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ~CoreInstance();

		/*
		 Steps the Cores logic.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus Step();

		/*
		 Only applies to System cores.
		 Is called when the System initially starts running, from the System core thread.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus SystemInit();

		/*
		 Only applies to System cores.
		 Returns whether or not the RunLoop should create a new thread for each Core instance loaded in this Core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual bool isMultithreaded();

		/*
		 Returns a pointer to this instances Core State.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual CoreState* getCoreState();
	};
}
