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
		std::thread runThread;

		// Synchronization for Multithreading, to insure that the System doesn't start until all the Cores have started, coming out of a pause.
		std::atomic<bool> systemReady;

		struct CoreInfo
		{
			std::thread thread;
			std::atomic<bool> ready;

			CoreInfo(std::thread&& thread)
			{
				this->thread.swap(thread);
			}

			CoreInfo(const CoreInfo& other)
			{
				CoreInfo& otherRef = const_cast<CoreInfo&>(other);
				thread.swap(otherRef.thread);

				ready.store(other.ready.load());
			}
		};
		std::vector<CoreInfo> cores;

		// The running and paused variables.
		std::atomic<bool> running;
		std::atomic<bool> paused;

		/*
		 The current System stepPeriod for Single-Threading 
		 (equals the smallest step for any of the cores).
		*/
		std::chrono::nanoseconds stepPeriod;

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
		 and one without that just loops through all the cores, suitable for simpler systems.
		 (Should be noted that individual cores can still create new threads even without multithreading enabled, 
		 it's just that they'll have to handle timing / pausing / synchronization themselves)
		*/
		template <bool multithreaded = false>
		void Loop()
		{
			systemInstance->Init();
			if constexpr (!multithreaded)
			{
				for (const std::shared_ptr<CoreInstance>& child : systemInstance->GetChildren())
					child->Init();
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
						for (CoreInfo& core : cores)
						{
							allCoresReady &= core.ready;
							if (!core.ready) break;
						}

						if (!allCoresReady) continue;
						else systemReady = true;
					}
				}

				auto startTime = std::chrono::steady_clock::now();
				if (startTime >= systemTimingInfo.nextStep)
				{
					if (systemInstance->Step() != CoreInstance::ReturnStatus::Success)
						Pause();
				}
				std::chrono::nanoseconds systemPeriod = systemInstance->GetStepPeriod();

				auto childrenStartTime = std::chrono::steady_clock::now();
				systemTimingInfo.nextStep = childrenStartTime + systemPeriod;
				if (systemPeriod < stepPeriod) stepPeriod = systemPeriod;
				
				if constexpr (!multithreaded)
				{
					if (!paused && !systemInstance->paused)
					{
						for (u32 i = 0; i < systemInstance->GetChildren().size(); ++i)
						{
							const std::shared_ptr<CoreInstance>& child = systemInstance->GetChildren()[i];
							if (!child->paused)
							{
								if (childrenStartTime >= timingInfo[i].nextStep)
								{
									if (child->Step() != CoreInstance::ReturnStatus::Success)
										Pause();
								}
							}

							std::chrono::nanoseconds instancePeriod = child->GetStepPeriod();
							timingInfo[i].nextStep = std::chrono::steady_clock::now() + instancePeriod;
							if (instancePeriod < stepPeriod) stepPeriod = instancePeriod;

							if (paused || systemInstance->paused) break;
						}
					}
				}

				if (stepPeriod > std::chrono::nanoseconds::zero())
				{
					auto endTime = std::chrono::steady_clock::now();

					WaitForPeriod(stepPeriod, startTime, endTime);
				}
			}
		}

		/*
		 The Loop implementation for individual Cores when Multithreading.
		*/
		void LoopMultithreadedCore(const std::shared_ptr<CoreInstance>& instance, s32 coreIndex);

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
		InstanceTimingInfo systemTimingInfo;
	public:
		Core* currentSystem;
		std::shared_ptr<SystemInstance> systemInstance;

		LIBGAMEEMU_COMMON_DLL_EXPORT RunLoop(LoggerManager& logManager, Core* system);

		LIBGAMEEMU_COMMON_DLL_EXPORT ~RunLoop();

		/*
		 Starts the main system thread and runs the current system core.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void Start(const std::unordered_map<std::string, PropertyValue>& propertyOverrides = {});

		/*
 		 Sets the system core to be run after calling Start.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void SetSystemCore(Core* systemCore);

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
		LIBGAMEEMU_COMMON_DLL_EXPORT bool IsPaused();

		/*
		 Returns whether or not the current System Core is running.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT bool IsRunning();
	};
}
