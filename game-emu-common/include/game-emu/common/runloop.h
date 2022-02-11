#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/symbolhelper.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/runstate.h>

#include <game-emu/common/coreinstance.h>

namespace GameEmu::Common
{
	class Core;

	class RunLoop : public RunState
	{
	private:
		// Common Variables for both Single-Threading, and Multithreading.
		std::mutex threadMutex;
		std::thread runThread;

		// Multithreading
		std::vector<std::thread> coreThreads;
		std::vector<std::unique_ptr<std::mutex>> coreMutexes;

		// Synchronization for Multithreading, to insure that the System doesn't start until all the Cores have started, coming out of a pause.
		std::atomic<bool> systemReady;
		std::vector<std::unique_ptr<std::atomic<bool>>> coresReady;

		// The running and paused variables.
		std::atomic<bool> running;
		std::atomic<bool> paused;

		/*
		 Wait at the end of a Core loop to pad the time 
		 if the Core step function was much faster than the real time it takes on hardware to step.
		*/
		inline void WaitForPeriod(std::chrono::nanoseconds period,
			std::chrono::time_point<std::chrono::steady_clock> startTime,
			std::chrono::time_point<std::chrono::steady_clock> endTime)
		{
			/*
			 Why not use sleep_until?
			 Because doing this "ugly" wait, will actually be more accurate as we won't give the OS time to do anything else.
			 This does use more resources but is important for more accurate timing.
			*/
			auto endTimeNS = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTime);
			auto startTimeNS = std::chrono::time_point_cast<std::chrono::nanoseconds>(startTime);
			while ((std::chrono::steady_clock::now() - endTime) < (period - (endTimeNS - startTimeNS))) continue;
		}

		/*
		 The Run Loop, with two templated versions. One for multithreading with extra synchronization checks, 
		 and one without that just forloops through all the cores, suitable for simpler systems.
		 (Should be noted that individual cores can still create new threads even without multithreading enabled, 
		 it's just that they'll have to handle timing / pausing / synchronization themselves)
		*/
		template <bool multithreaded = false>
		void Loop()
		{
			systemInstance->Init();
			if constexpr (!multithreaded)
			{
				for (const std::shared_ptr<CoreInstance>& instance : systemInstance->getInstances())
					instance->Init();
			}

			while (running)
			{
				if (paused || systemInstance->paused)
				{
					if constexpr (multithreaded) systemReady = false;
					std::this_thread::sleep_for(std::chrono::milliseconds(150));
					continue;
				}

				if constexpr (multithreaded)
				{
					if (!systemReady)
					{
						bool allCoresReady = true;
						for (std::unique_ptr<std::atomic<bool>>& coreReady : coresReady)
						{
							allCoresReady &= *coreReady;
							if (!*coreReady) break;
						}

						if (!allCoresReady) continue;
						else systemReady = true;
					}
				}

				threadMutex.lock();
				auto startTime = std::chrono::steady_clock::now();

				if constexpr (!multithreaded)
				{
					for (u32 i = 0; i < systemInstance->getInstances().size(); ++i)
					{
						const std::shared_ptr<CoreInstance>& instance = systemInstance->getInstances()[i];
						if (!instance->paused)
						{
							if (std::chrono::steady_clock::now() >= timingInfo[i].nextStep)
							{
								if (instance->Step() != CoreInstance::ReturnStatus::Success)
									Pause();
							}
							timingInfo[i].nextStep = std::chrono::steady_clock::now() + instance->getStepPeriod();
						}
					}
				}

				threadMutex.unlock();
				auto endTime = std::chrono::steady_clock::now();

				WaitForPeriod(stepPeriod, startTime, endTime);
			}
		}

		/*
		 The Loop implementation for individual Cores when Multithreading.
		*/
		void LoopMultithreadedCore(const std::shared_ptr<CoreInstance>& instance, s32 coreIndex);

		/*
		 The current System stepPeriod for Single-Threading (equals the smallest step for any of the cores).
		*/
		std::chrono::nanoseconds stepPeriod;

		/*
		 Timing info for each Core, stores when the next step for that core is.
		*/
		struct InstanceTimingInfo
		{
			std::chrono::time_point<std::chrono::steady_clock> nextStep;

			InstanceTimingInfo()
			{
				nextStep = std::chrono::steady_clock::now();
			}

			InstanceTimingInfo(std::chrono::time_point<std::chrono::steady_clock> nextStep)
			{
				this->nextStep = nextStep;
			}
		};
		std::vector<InstanceTimingInfo> timingInfo;
	public:
		Core* currentSystem;
		std::shared_ptr<CoreInstance> systemInstance;

		LIBGAMEEMU_COMMON_DLL_EXPORT RunLoop();
		LIBGAMEEMU_COMMON_DLL_EXPORT ~RunLoop();

		/*
		 Starts the main system thread and runs the current system core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void Start();

		/*
 		 Sets the system core to be run after calling Start.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void setSystemCore(Core* systemCore, std::unordered_map<std::string, PropertyValue> properties = {});

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
		LIBGAMEEMU_COMMON_DLL_EXPORT bool isPaused();

		/*
		 Returns whether or not the current System Core is running.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT bool isRunning();

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
