#include <game-emu/common/runloop.h>

#include <game-emu/common/core.h>
#include <game-emu/common/coreinstance.h>

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
	}

	void RunLoop::Loop()
	{
		systemInstance->SystemInit();

		while (running)
		{
			if (paused || systemInstance->paused)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(80));
				continue;
			}

			threadMutex.lock();

			systemInstance->Step();

			if (!systemInstance->isMultithreaded())
			{
				for (const std::unique_ptr<CoreInstance>& instance : systemInstance->getInstances())
					if (!instance->paused) instance->Step();
			}

			threadMutex.unlock();
		}
	}

	void RunLoop::LoopCore(const std::unique_ptr<CoreInstance>& instance, int mutexIndex)
	{
		while (running)
		{
			if (paused || instance->paused)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(80));
				continue;
			}

			coreMutexes[mutexIndex]->lock();
			instance->Step();
			coreMutexes[mutexIndex]->unlock();
		}
	}

	void RunLoop::Start(std::unordered_map<std::string, PropertyValue> properties)
	{
		if (currentSystem)
		{
			if (!running && runThread.joinable())
				runThread.join();

			if (systemInstance) delete systemInstance.release();
			systemInstance = currentSystem->createNewInstance(*this, properties);

			running = true;
			runThread = std::thread(&RunLoop::Loop, this);
			if (systemInstance->isMultithreaded())
			{
				for (const std::unique_ptr<CoreInstance>& instance : systemInstance->getInstances())
				{
					coreMutexes.push_back(std::make_unique<std::mutex>());
					coreThreads.push_back(std::thread(&RunLoop::LoopCore, this, std::ref(instance), (int)coreMutexes.size() - 1));
				}
			}
		}
	}

	void RunLoop::setSystemCore(Core* core)
	{
		if (core->getType() != Core::Type::System) return;
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
