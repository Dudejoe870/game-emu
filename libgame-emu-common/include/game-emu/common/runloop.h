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
		CoreInstance* systemInstance;

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
		LIBGAMEEMU_COMMON_DLL_EXPORT void Stop();

		/*
		 Pauses the current loop.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void Pause();

		/*
		 Resumes the current loop if it has been previously paused.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void Resume();

		/*
		 Returns whether or not the current System Core is running.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT bool isRunning();

		/*
		 This acquires the thread mutex basically stopping any Cores from running while the caller accesses any current Core states.
		 It is heavily recommended to call this if accessing any Core states from outside threads (example: for debugging purposes)
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void AcquireLock();

		/*
		 Unlocks the thread mutex. Call this after finishing access to the Core state.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void Unlock();
	};
}
