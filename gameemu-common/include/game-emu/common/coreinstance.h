#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/symbolhelper.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/runstate.h>

#include <game-emu/common/corestate.h>

#include <game-emu/common/physicalmemorymap.h>

namespace GameEmu::Common
{
	class Core;

	class CoreInstance
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<PhysicalMemoryMap>> physicalMemoryMaps;
	protected:
		Core* core;
		RunState& runState;

		std::shared_ptr<PhysicalMemoryMap> GetMemoryMap(const std::string& name)
		{
			return physicalMemoryMaps[name];
		}
	public:
		std::atomic<bool> paused;

		enum class ReturnStatus
		{
			Success,
			Error,

			UnknownInstruction
		};

		/*
		 Adds an address space with a name.
		*/
		inline void AddMemoryMap(std::shared_ptr<PhysicalMemoryMap> memoryMap, const std::string& name)
		{
			physicalMemoryMaps[name] = memoryMap;
		}

		/*
		 Gets the map of address spaces and names associated with this instance.
		*/
		inline const std::unordered_map<std::string, std::shared_ptr<PhysicalMemoryMap>>& GetMemoryMaps() const
		{
			return physicalMemoryMaps;
		}

		/*
		 Gets the core associated with this instance.
		*/
		inline const Core* GetCore() const
		{
			return core;
		}

		// KeyValue map Properties for this particular Core instance, value can be an int, string, float, or bool of any size.
		std::unordered_map<std::string, PropertyValue> properties;
		
		LIBGAMEEMU_COMMON_DLL_EXPORT CoreInstance(Core* core, RunState& runState,
			const std::unordered_map<std::string, PropertyValue>& propertyOverrides);

		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ~CoreInstance();

		/*
		 Steps the Cores logic.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus Step();

		/*
		 Is called when the Core initially starts running.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus Init();

		/*
		 Returns a const reference to this instances Core State.
		*/
		virtual const CoreState& GetCoreState() const = 0;
		
		/*
		 Returns this instances stepping period.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::chrono::nanoseconds GetStepPeriod() const;
	};

	class SystemInstance : public CoreInstance
	{
	private:
		std::vector<std::shared_ptr<CoreInstance>> children;

		LIBGAMEEMU_COMMON_DLL_EXPORT u64 AddChildImpl(Core* dependency,
			const std::unordered_map<std::string, PropertyValue>& propertyOverrides);
	protected:
		/*
		 Adds a new child core instance and returns the newly added instance.
		*/
		template <class T>
		std::shared_ptr<T> AddChild(Core* dependency, const std::unordered_map<std::string, PropertyValue>& propertyOverrides = {})
		{
			return std::dynamic_pointer_cast<T>(children[AddChildImpl(dependency, propertyOverrides)]);
		}
	public:
		SystemInstance(Core* core, RunState& runState,
			const std::unordered_map<std::string, PropertyValue>& propertyOverrides)
			: CoreInstance(core, runState, propertyOverrides)
		{
		}

		/*
		 Gets the child instances associated with this instance.
		*/
		inline const std::vector<std::shared_ptr<CoreInstance>>& GetChildren() const
		{
			return children;
		}

		/*
		 Returns whether or not the RunLoop should create a new thread for each Core instance loaded in this Core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual bool IsMultithreaded() const;
	};
}
