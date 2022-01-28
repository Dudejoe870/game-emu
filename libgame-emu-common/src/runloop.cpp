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
		if (systemInstance) delete systemInstance;
	}

	void RunLoop::Loop()
	{
		systemInstance->SystemInit();

		while (running.test())
		{
			if (paused.test())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(80));
				continue;
			}

			threadMutex.lock();
			systemInstance->Step();
			threadMutex.unlock();
		}
	}

	void RunLoop::Start(std::unordered_map<std::string, PropertyValue> properties)
	{
		if (currentSystem)
		{
			if (!running.test() && runThread.joinable())
				runThread.join();

			if (systemInstance) delete systemInstance;
			systemInstance = currentSystem->createNewInstance(properties);

			running.test_and_set();
			runThread = std::thread(&RunLoop::Loop, this);
		}
	}

	void RunLoop::setSystemCore(Core* core)
	{
		if (core->getType() != Core::Type::System) return;
		this->currentSystem = core;
	}

	void RunLoop::AcquireLock()
	{
		threadMutex.lock();
	}

	void RunLoop::Unlock()
	{
		threadMutex.unlock();
	}

	void RunLoop::Stop()
	{
		running.clear();
	}

	void RunLoop::Pause()
	{
		paused.test_and_set();
	}

	void RunLoop::Resume()
	{
		paused.clear();
	}

	bool RunLoop::isRunning()
	{
		return running.test();
	}
}
