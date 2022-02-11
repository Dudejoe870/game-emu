/*
 game-emu-cli: A CLI tool for using and debugging game-emu cores built off the game-emu-common library.

	Copyright 2022 John Henry Clemis

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to
	whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial
	portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
	TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/core.h>
#include <game-emu/common/coreloader.h>
#include <game-emu/common/coreinstance.h>
#include <game-emu/common/instructionbasedcoreinstance.h>

#include <game-emu/common/runloop.h>
#include <game-emu/common/logger.h>

#include <args.hxx>

#include <cli/cli.h>
#include <cli/loopscheduler.h>
#include <cli/clilocalsession.h>

using namespace GameEmu;

class CLILogger : public Common::Logger 
{
private:
	cli::Scheduler& sched;
protected:
	void LogInfoImpl(std::string info)
	{
		sched.Post([info]() { cli::Cli::cout() << "Info: " << info << std::endl; });
	}

	void LogWarningImpl(std::string warning)
	{
		sched.Post([warning]() { cli::Cli::cout() << "Warning: " << warning << std::endl; });
	}

	void LogErrorImpl(std::string error)
	{
		sched.Post([error]() { cli::Cli::cout() << "Error: " << error << std::endl; });
	}
public:
	CLILogger(cli::Scheduler& sched)
		: sched(sched)
	{
	}
};

void ParseCore(const std::string& progName, Common::Core* core, std::vector<std::string>::const_iterator beginArgs, std::vector<std::string>::const_iterator endArgs)
{
	std::unordered_map<std::string, Common::PropertyValue> defaultProperties = core->getDefaultProperties();

	std::unordered_map<std::string, Common::PropertyValue> propertyOverrides;
	propertyOverrides.insert(defaultProperties.begin(), defaultProperties.end());

	args::ArgumentParser parser(core->getDescription());
	parser.Prog(progName + " " + core->getName());
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });

	args::Flag pauseOnStart(parser, "pause-on-start", "Pauses the system core as soon as it starts.", { 'p', "pause-on-start" });

	args::Group propertiesGroup(parser, "Properties");

	// A List of all the properties associated with pointers to CLI flag objects.
	std::unordered_map<std::string, std::unique_ptr<args::ValueFlag<std::string>>> propertyFlags;
	for (const auto& kv : defaultProperties)
	{
		propertyFlags[kv.first] = std::make_unique<args::ValueFlag<std::string>>(
			propertiesGroup,
			kv.first, 
			"(" + Common::Util::getPropertyValueTypeDisplayName(Common::Util::getPropertyValueType(kv.second)) + ")",
			args::Matcher { kv.first });
	}

	try
	{
		parser.ParseArgs(beginArgs, endArgs);

		if (beginArgs == endArgs) std::cout << parser;
		for (auto& kv : propertyFlags)
		{
			std::unique_ptr<args::ValueFlag<std::string>>& flag = kv.second;
			Common::PropertyValue& value = propertyOverrides[kv.first];
			Common::Util::PropertyValueType type = Common::Util::getPropertyValueType(value);
			if (*flag.get())
			{
				try 
				{
					std::string lowerCaseFlag = flag->Get();
					std::transform(lowerCaseFlag.begin(), lowerCaseFlag.end(), lowerCaseFlag.begin(),
						[](u8 c) { return std::tolower(c); }); // This will break with UTF encoding.

					// Parse each PropertyValue type to set the PropertyValue correctly.
					switch (type)
					{
					case Common::Util::PropertyValueType::Integer32:
						value = (s32)std::stoi(flag->Get());
						break;
					case Common::Util::PropertyValueType::Integer64:
						value = (s64)std::stoll(flag->Get());
						break;
					case Common::Util::PropertyValueType::Integer8:
						value = (s8)std::stoi(flag->Get());
						break;
					case Common::Util::PropertyValueType::Integer16:
						value = (s16)std::stoi(flag->Get());
						break;
					case Common::Util::PropertyValueType::Float:
						value = (float)std::stof(flag->Get());
						break;
					case Common::Util::PropertyValueType::Double:
						value = (double)std::stod(flag->Get());
						break;
					case Common::Util::PropertyValueType::Bool:
						if (lowerCaseFlag == "true") value = true;
						else if (lowerCaseFlag == "false") value = false;
						else throw std::invalid_argument("");
						break;
					default:
					case Common::Util::PropertyValueType::String:
						value = flag->Get();
						break;
					}
				}
				catch (const std::invalid_argument&)
				{
					std::cerr << "Couldn't parse flag " << kv.first << "." << std::endl;
					return;
				}
			}
			else propertyOverrides.erase(kv.first);
		}
	}
	catch (const args::Help&)
	{
		std::cout << parser;
		return;
	}
	catch (const args::ParseError& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return;
	}

	cli::LoopScheduler sched;
	CLILogger logger(sched);

	Common::RunLoop loop;
	bool useRunLoop = false;

	if (beginArgs != endArgs)
	{
		useRunLoop = true;

		loop.logger = &logger;
		loop.setSystemCore(core, propertyOverrides);

		cli::SetColor();

		auto rootMenu = std::make_unique<cli::Menu>(core->getName());

		rootMenu->Insert("resume",
			[&](std::ostream& o)
			{
				o << "Resuming " << core->getName() << "..." << std::endl;
				loop.Resume();
			}, "Starts the core.");
		rootMenu->Insert("pause",
			[&](std::ostream& o)
			{
				o << "Pausing " << core->getName() << "..." << std::endl;
				loop.Pause();
			}, "Pauses the core.");
		rootMenu->Insert("status",
			[&](std::ostream& o)
			{
				std::string status = "stopped";
				if (loop.isPaused()) status = "paused";
				else if (loop.isRunning()) status = "running";
				o << status << std::endl;
			}, "Gets the status of the core.");

		Common::CoreState* systemCoreState = loop.systemInstance->getCoreState();
		if (systemCoreState)
		{
			rootMenu->Insert("state",
				[&](std::ostream& o)
				{
					loop.AcquireLock();

					for (Common::CoreState::DebugRegisterInfo& info : systemCoreState->getRegisters())
						o << info.getFormatted() << std::endl;

					loop.Unlock();
				}, "Display the core state registers.");
		}

		std::unordered_map<Common::Core*, u32> duplicateCounts;
		for (const std::shared_ptr<Common::CoreInstance>& instance : loop.systemInstance->getInstances())
		{
			Common::InstructionBasedCoreInstance* instructionBasedInstance = dynamic_cast<Common::InstructionBasedCoreInstance*>(instance.get());

			std::string coreName = instance->getCore()->getName();
			if (duplicateCounts[instance->getCore()]++ > 0) coreName += "-" + std::to_string(duplicateCounts[instance->getCore()]);

			auto coreMenu = std::make_unique<cli::Menu>(coreName);

			coreMenu->Insert("info",
				[&](std::ostream& o) 
				{
					o << "Name: " << instance->getCore()->getName() << std::endl;
					o << "Description: " << instance->getCore()->getDescription() << std::endl;
					o << "Is Instruction Based: " << (instructionBasedInstance ? "Yes" : "No") << std::endl;
				}, "Display general info about this core.");

			Common::CoreState* coreState = instance->getCoreState();
			if (coreState)
			{
				coreMenu->Insert("state",
					[&](std::ostream& o)
					{
						loop.AcquireLock();

						for (Common::CoreState::DebugRegisterInfo& info : coreState->getRegisters())
							o << info.getFormatted() << std::endl;

						loop.Unlock();
					}, "Display Core State Registers.");
			}

			rootMenu->Insert(std::move(coreMenu));
		}

		cli::Cli cli(std::move(rootMenu));

		cli::CliLocalTerminalSession session(cli, sched, std::cout);

		session.ExitAction([&](std::ostream& o) 
		{ 
			loop.Stop();
			sched.Stop();
			o << "Goodbye..." << std::endl; 
		});

		if (useRunLoop)
		{
			if (pauseOnStart) loop.Pause();
			loop.Start();
		}

		sched.Run();
	}
}

int main(int argc, char** argv)
{
	Common::CoreLoader coreLoader("game-emu-cores");

	const std::vector<Common::Core*>& loadedCores = coreLoader.getLoadedCores();

	std::unordered_map<std::string, Common::Core*> cores;
	for (Common::Core* core : loadedCores)
		if (core->getType() == Common::Core::Type::System) cores[core->getName()] = core;

	const std::vector<std::string> args(argv + 1, argv + argc);

	args::ArgumentParser parser("A CLI Interface for a common library for building game system emulation cores.", "Copyright 2022 John Henry Clemis");
	parser.Prog(std::string(argv[0]) + " {CLI OPTIONS}");

	args::Group cliOpts(parser, "CLI OPTIONS");
	args::HelpFlag help(cliOpts, "help", "Display this help menu", { 'h', "help" });

	args::Flag showCores(cliOpts, "show-cores", "Shows all available system cores that can be used.", { "show-cores" });
	showCores.KickOut(true);

	args::Flag showAllCores(cliOpts, "show-all-cores", "Shows all cores that can be currently loaded, including ones that are only loadable by other cores.", { "show-all-cores" });
	showAllCores.KickOut(true);

	args::MapPositional<std::string, Common::Core*> core(cliOpts, "CORE", "Core to run.", cores);
	core.KickOut(true);

	try
	{
		auto next = parser.ParseArgs(args);
		if (core) ParseCore(argv[0], args::get(core), next, std::end(args));
		else if (showCores)
		{
			std::cout << "Cores: " << std::endl;
			for (Common::Core* loadedCore : loadedCores)
			{
				if (loadedCore->getType() == Common::Core::Type::System)
					std::cout << " " << loadedCore->getName() << ": " << loadedCore->getDescription() << std::endl;
			}
		}
		else if (showAllCores)
		{
			std::cout << "Cores: " << std::endl;
			for (Common::Core* loadedCore : loadedCores)
			{
				std::string coreType = "";
				switch (loadedCore->getType())
				{
				case Common::Core::Type::Processor:
					coreType = "(Processor)";
					break;
				case Common::Core::Type::GraphicsRasterizer:
					coreType = "(Graphics Rasterizer)";
					break;
				case Common::Core::Type::System:
					coreType = "(System)";
					break;
				}
				std::cout << " " << loadedCore->getName() << ": " << loadedCore->getDescription() << " " << coreType << std::endl;
			}
		}
		else std::cout << parser;
	}
	catch (const args::Help&)
	{
		std::cout << parser;
		return 0;
	}
	catch (const args::Error& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}

	return 0;
}
