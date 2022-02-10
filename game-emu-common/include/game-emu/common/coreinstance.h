#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/symbolhelper.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/runstate.h>

#include <game-emu/common/corestate.h>

#include <game-emu/common/physicaladdresstranslator.h>

namespace GameEmu::Common
{
	class Core;

	class CoreInstance
	{
	private:
		std::vector<std::shared_ptr<CoreInstance>> instances;
		std::unordered_map<std::string, PhysicalAddressTranslator*> physicalAddressSpaces;

		LIBGAMEEMU_COMMON_DLL_EXPORT u64 addInstanceImpl(Core* dependency, const std::unordered_map<std::string, PropertyValue>& properties);
	protected:
		Core* core;

		RunState& runState;

		/*
		 Adds a new child core instance and returns the newly added instance.
		*/
		template <class T>
		T* addInstance(Core* dependency, const std::unordered_map<std::string, PropertyValue>& properties = {})
		{
			return reinterpret_cast<T*>(instances[addInstanceImpl(dependency, properties)].get());
		}

		PhysicalAddressTranslator* getAddressSpace(const std::string& name)
		{
			if (!physicalAddressSpaces.contains(name)) return nullptr;
			return physicalAddressSpaces[name];
		}
	public:
		std::atomic<bool> paused;

		enum class ReturnStatus
		{
			Success,
			Error
		};

		/*
		 Adds an address space with a name.
		*/
		inline void addAddressSpace(PhysicalAddressTranslator& addressSpace, std::string name)
		{
			physicalAddressSpaces[name] = &addressSpace;
		}

		/*
		 Gets the map of address spaces and names associated with this instance.
		*/
		inline const std::unordered_map<std::string, PhysicalAddressTranslator*>& getAddressSpaces()
		{
			return physicalAddressSpaces;
		}

		/*
		 Gets the child instances associated with this instance.
		*/
		inline const std::vector<std::shared_ptr<CoreInstance>>& getInstances()
		{
			return instances;
		}

		/*
		 Gets the core associated with this instance.
		*/
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
		 Is called when the Core initially starts running.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus Init();

		/*
		 Only applies to System cores.
		 Returns whether or not the RunLoop should create a new thread for each Core instance loaded in this Core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual bool isMultithreaded();

		/*
		 Returns a pointer to this instances Core State.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual CoreState* getCoreState();
		
		/*
		 Returns this instances stepping period.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual std::chrono::nanoseconds getStepPeriod();
	};
}
