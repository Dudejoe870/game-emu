#include <iostream>

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/core.h>
#include <game-emu/common/coreloader.h>
#include <game-emu/common/coreinstance.h>

#include <game-emu/common/runloop.h>

#include <args.hxx>

#include <cli/cli.h>
#include <cli/loopscheduler.h>
#include <cli/clilocalsession.h>

using namespace GameEmu;

void ParseCore(const std::string& progName, Common::Core* core, std::vector<std::string>::const_iterator beginArgs, std::vector<std::string>::const_iterator endArgs)
{
	std::unique_ptr<Common::CoreInstance> tempInstance = core->createNewInstance();
	std::unordered_map<std::string, Common::PropertyValue> propertyOverrides;
	propertyOverrides.insert(tempInstance->properties.begin(), tempInstance->properties.end());

	args::ArgumentParser parser(core->getDescription());
	parser.Prog(progName + " " + core->getName());
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });

	args::Flag pauseOnStart(parser, "pause-on-start", "Pauses the system core as soon as it starts.", { 'p', "pause-on-start" });

	// A List of all the properties associated with pointers to CLI flag objects.
	std::unordered_map<std::string, std::unique_ptr<args::ValueFlag<std::string>>> propertyFlags;
	for (const auto& kv : tempInstance->properties)
	{
		propertyFlags[kv.first] = std::make_unique<args::ValueFlag<std::string>>(
			parser, 
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
				try {
					std::string lowerCaseFlag = flag->Get();
					std::transform(lowerCaseFlag.begin(), lowerCaseFlag.end(), lowerCaseFlag.begin(),
						[](unsigned char c) { return std::tolower(c); }); // This will break with UTF encoding.

					// Parse each PropertyValue type to set the PropertyValue correctly.
					switch (type)
					{
					case Common::Util::PropertyValueType::Integer32:
						value = (int)std::stoi(flag->Get());
						break;
					case Common::Util::PropertyValueType::Integer64:
						value = (long long)std::stoll(flag->Get());
						break;
					case Common::Util::PropertyValueType::Integer8:
						value = (char)std::stoi(flag->Get());
						break;
					case Common::Util::PropertyValueType::Integer16:
						value = (short)std::stoi(flag->Get());
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
					break;
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

	if (beginArgs != endArgs)
	{
		Common::RunLoop loop;
		loop.setSystemCore(core);

		if (pauseOnStart) loop.Pause();
		loop.Start(propertyOverrides);

		cli::SetColor();

		auto rootMenu = std::make_unique<cli::Menu>("debug");

		std::unordered_map<Common::Core*, int> duplicateCounts;
		for (const std::unique_ptr<Common::CoreInstance>& instance : loop.systemInstance->getInstances())
		{
			auto coreMenu = std::make_unique<cli::Menu>(instance->getCore()->getName() + "-" + std::to_string(duplicateCounts[instance->getCore()]++));

			coreMenu->Insert("info",
				[&](std::ostream& o) 
				{
					o << "Name: " << instance->getCore()->getName() << std::endl;
					o << "Dependencies:" << std::endl;
					if (instance->getCore()->getDependencies().empty()) o << " None" << std::endl;
					for (Common::Core* core : instance->getCore()->getDependencies())
						o << " " << core->getName() << std::endl;

				}, "Display general info about this core.");

			rootMenu->Insert(std::move(coreMenu));
		}

		cli::Cli cli(std::move(rootMenu));

		cli::LoopScheduler sched;
		cli::CliLocalTerminalSession session(cli, sched, std::cout);

		session.ExitAction([&](std::ostream& o) 
		{ 
			loop.Stop();
			sched.Stop();
			o << "Goodbye..." << std::endl; 
		});

		sched.Run();
	}
}

int main(int argc, char** argv)
{
	Common::CoreLoader coreLoader("cores");

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
			for (Common::Core* core : loadedCores)
			{
				if (core->getType() == Common::Core::Type::System)
					std::cout << " " << core->getName() << ": " << core->getDescription() << std::endl;
			}
		}
		else if (showAllCores)
		{
			std::cout << "Cores: " << std::endl;
			for (Common::Core* core : loadedCores)
			{
				std::string coreType = "";
				switch (core->getType())
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
				std::cout << " " << core->getName() << 
					((core->getType() == Common::Core::Type::System) ? ": " + core->getDescription() + " " : " ")
					<< coreType << std::endl;
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
