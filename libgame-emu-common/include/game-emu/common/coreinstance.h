#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/runstate.h>

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
		 Returns a disassembled string with each instruction separated by a new line.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::string Disassemble(const std::vector<unsigned char>& data);

		/*
		 Returns whether or not this Core can disassemble machine code.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual bool canDisassemble();

		/*
		 Only applies to System cores.
		 Is called when the System initially starts running, from the System core thread.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus SystemInit();

		/*
		 Returns whether or not the RunLoop should create a new thread for each Core instance loaded in this Core. (Only applies to System Cores)
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual bool isMultithreaded();
	};
}
