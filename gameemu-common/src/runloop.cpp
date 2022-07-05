#include <game-emu/common/runloop.h>

#include <game-emu/common/core.h>

namespace GameEmu::Common
{
	RunLoop::RunLoop(LoggerManager& logManager, Core* system)
		: RunState(logManager)
	{
		SetSystemCore(system);
		this->systemInstance = nullptr;
	}

	RunLoop::~RunLoop()
	{
		if (runThread.joinable()) runThread.join();
		for (CoreInfo& core : cores)
			if (core.thread.joinable()) core.thread.join();
	}

	void RunLoop::LoopMultithreadedCore(const std::shared_ptr<CoreInstance>& instance, s32 coreIndex)
	{
		instance->Init();

		while (running)
		{
			if (paused)
			{
				cores[coreIndex].ready = false;
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				continue;
			}
			cores[coreIndex].ready = true;

			if (!systemReady) continue;

			if (instance->paused)
			{
				/*
				 We can't sleep when the instance is paused because if it's unpaused, it may need to be unpaused immediately.
				*/
				continue;
			}

			auto startTime = std::chrono::steady_clock::now();

			if (instance->Step() != CoreInstance::ReturnStatus::Success)
				Pause();

			auto endTime = std::chrono::steady_clock::now();

			WaitForPeriod(instance->GetStepPeriod(), startTime, endTime);
		}
	}

	void RunLoop::Start(const std::unordered_map<std::string, PropertyValue>& propertyOverrides)
	{
		if (currentSystem)
		{
			if (!running && runThread.joinable())
				runThread.join();
			if (systemInstance && systemInstance->IsMultithreaded())
			{
				for (CoreInfo& core : cores)
					if (!running && core.thread.joinable()) core.thread.join();
			}

			this->systemInstance = std::dynamic_pointer_cast<SystemInstance>(
				currentSystem->CreateNewInstance(*this, propertyOverrides));
			if (!this->systemInstance)
				throw std::runtime_error("SetSystemCore: Core isn't of a System type!");

			running = true;

			stepPeriod = std::chrono::nanoseconds::zero();
			for (const std::shared_ptr<CoreInstance>& child : systemInstance->GetChildren())
			{
				if (systemInstance->IsMultithreaded())
				{
					cores.push_back(CoreInfo(
						std::thread(&RunLoop::LoopMultithreadedCore, this, std::ref(child), static_cast<s32>(cores.size()) - 1)));
				}

				std::chrono::nanoseconds instancePeriod = child->GetStepPeriod();
				if (instancePeriod < stepPeriod || stepPeriod == std::chrono::nanoseconds::zero()) stepPeriod = instancePeriod;
			}

			logger->LogInfo(fmt::format("RunLoop starting system {}...", currentSystem->GetName()));

			if (systemInstance->IsMultithreaded())
			{
				systemReady = false;
				runThread = std::thread(&RunLoop::Loop<true>, this);
			}
			else
			{
				timingInfo.resize(systemInstance->GetChildren().size());
				runThread = std::thread(&RunLoop::Loop<false>, this);
			}
		}
	}

	void RunLoop::SetSystemCore(Core* systemCore)
	{
		if (systemCore->GetType() != Core::Type::System) 
			throw std::runtime_error("SetSystemCore: Core isn't of a System type!");
		this->currentSystem = systemCore;
	}

	bool RunLoop::IsRunning()
	{
		return running;
	}

	bool RunLoop::IsPaused()
	{
		return paused;
	}
}
