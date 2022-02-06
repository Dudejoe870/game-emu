#include <game-emu/common/runloop.h>

#include <game-emu/common/core.h>

namespace GameEmu::Common
{
	RunLoop::RunLoop()
	{
		this->currentSystem = nullptr;
		this->systemInstance = nullptr;
	}

	RunLoop::~RunLoop()
	{
		if (runThread.joinable()) runThread.join();
		for (std::thread& thread : coreThreads)
			if (thread.joinable()) thread.join();
	}

	void RunLoop::LoopMultithreaded()
	{
		LoopImpl<true>();
	}

	void RunLoop::Loop()
	{
		LoopImpl<false>();
	}

	void RunLoop::LoopMultithreadedCore(const std::unique_ptr<CoreInstance>& instance, int coreIndex)
	{
		while (running)
		{
			if (paused)
			{
				*coresReady[coreIndex] = false;
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				continue;
			}
			*coresReady[coreIndex] = true;

			if (!systemReady) continue;

			if (instance->paused)
			{
				/*
				 We can't sleep when the instance is paused because if it's unpaused, it may need to be unpaused immediately.
				*/
				continue;
			}

			coreMutexes[coreIndex]->lock();
			auto startTime = std::chrono::steady_clock::now();

			instance->Step();

			coreMutexes[coreIndex]->unlock();
			auto endTime = std::chrono::steady_clock::now();

			WaitForPeriod(instance->getStepPeriod(), startTime, endTime);
		}
	}

	void RunLoop::Start(std::unordered_map<std::string, PropertyValue> properties)
	{
		if (currentSystem)
		{
			if (!running && runThread.joinable())
				runThread.join();
			if (systemInstance && systemInstance->isMultithreaded())
			{
				for (std::thread& thread : coreThreads)
					if (!running && thread.joinable()) thread.join();
			}

			if (systemInstance) delete systemInstance.release();
			systemInstance = currentSystem->createNewInstance(*this, properties);

			running = true;

			stepPeriod = std::chrono::nanoseconds::zero();
			for (const std::unique_ptr<CoreInstance>& instance : systemInstance->getInstances())
			{
				if (systemInstance->isMultithreaded())
				{
					coreMutexes.push_back(std::make_unique<std::mutex>());
					coresReady.push_back(std::make_unique<std::atomic<bool>>(false));
					coreThreads.push_back(std::thread(&RunLoop::LoopMultithreadedCore, this, std::ref(instance), (int)coreMutexes.size() - 1));
				}

				std::chrono::nanoseconds instancePeriod = instance->getStepPeriod();
				if (instancePeriod < stepPeriod || stepPeriod == std::chrono::nanoseconds::zero()) stepPeriod = instancePeriod;
			}

			if (systemInstance->isMultithreaded())
			{
				systemReady = true;
				runThread = std::thread(&RunLoop::LoopMultithreaded, this);
			}
			else
			{
				timingInfo.resize(systemInstance->getInstances().size());
				runThread = std::thread(&RunLoop::Loop, this);
			}
		}
	}

	void RunLoop::setSystemCore(Core* core)
	{
		if (core->getType() != Core::Type::System) throw std::runtime_error("setSystemCore: Core isn't of a System type!");
		this->currentSystem = core;
	}

	bool RunLoop::isRunning()
	{
		return running;
	}

	bool RunLoop::isPaused()
	{
		return paused;
	}
}
