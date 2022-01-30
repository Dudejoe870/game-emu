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
		std::vector<std::thread> coreThreads;
		std::vector<std::unique_ptr<std::mutex>> coreMutexes;

		std::atomic<bool> running;
		std::atomic<bool> paused;

		void Loop();
		void LoopCore(const std::unique_ptr<CoreInstance>& instance, int mutexIndex);
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
			running = false;
		}

		/*
		 Pauses the current loop.
		*/
		inline void Pause()
		{
			paused = true;
		}

		/*
		 Resumes the current loop if it has been previously paused.
		*/
		inline void Resume()
		{
			paused = false;
		}

		/*
		 Returns whether or not the current System Core is paused.
		*/
		inline bool isPaused()
		{
			return paused;
		}

		/*
		 Returns whether or not the current System Core is running.
		*/
		inline bool isRunning()
		{
			return running;
		}

		/*
		 This acquires the thread mutex basically stopping any Cores from running while the caller accesses any current Core states.
		 It is heavily recommended to call this if accessing any Core states from outside threads (example: for debugging purposes)
		*/
		inline void AcquireLock()
		{
			threadMutex.lock();
			for (std::unique_ptr<std::mutex>& mutex : coreMutexes) mutex->lock();
		}

		/*
		 Unlocks the thread mutex. Call this after finishing access to the Core state.
		*/
		inline void Unlock()
		{
			threadMutex.unlock();
			for (std::unique_ptr<std::mutex>& mutex : coreMutexes) mutex->unlock();
		}
	};
}
