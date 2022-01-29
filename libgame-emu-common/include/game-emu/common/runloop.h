#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/propertyvalue.h>

namespace GameEmu::Common
{
	class Core;
	class CoreInstance;

	class RunLoop
	{
	private:
		std::mutex threadMutex;
		std::thread runThread;

		std::atomic_flag running = ATOMIC_FLAG_INIT;
		std::atomic_flag paused = ATOMIC_FLAG_INIT;

		void Loop();
	public:
		Core* currentSystem;
		std::unique_ptr<CoreInstance> systemInstance;

		LIBGAMEEMU_COMMON_DLL_EXPORT RunLoop();
		LIBGAMEEMU_COMMON_DLL_EXPORT ~RunLoop();

		/*
		 Starts the main system thread and runs the current system core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void Start(std::unordered_map<std::string, PropertyValue> properties = {});

		/*
 		 Sets the system core to be run after calling Start.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void setSystemCore(Core* systemCore);

		/*
		 Stops the current loop.
		*/
		inline void Stop()
		{
			running.clear();
		}

		/*
		 Pauses the current loop.
		*/
		inline void Pause()
		{
			paused.test_and_set();
		}

		/*
		 Resumes the current loop if it has been previously paused.
		*/
		inline void Resume()
		{
			paused.clear();
		}

		/*
		 Returns whether or not the current System Core is paused.
		*/
		inline bool isPaused()
		{
			return paused.test();
		}

		/*
		 Returns whether or not the current System Core is running.
		*/
		inline bool isRunning()
		{
			return running.test();
		}

		/*
		 This acquires the thread mutex basically stopping any Cores from running while the caller accesses any current Core states.
		 It is heavily recommended to call this if accessing any Core states from outside threads (example: for debugging purposes)
		*/
		inline void AcquireLock()
		{
			threadMutex.lock();
		}

		/*
		 Unlocks the thread mutex. Call this after finishing access to the Core state.
		*/
		inline void Unlock()
		{
			threadMutex.unlock();
		}
	};
}
