#include <iostream>

#include <game-emu/common/core.h>
#include <game-emu/common/coreloader.h>
#include <game-emu/common/coreinstance.h>

#include <game-emu/common/runloop.h>

#include <args.hxx>

using namespace GameEmu;

void ParseCore(const std::string& progName, Common::Core* core, std::vector<std::string>::const_iterator beginArgs, std::vector<std::string>::const_iterator endArgs)
{
	args::ArgumentParser parser("");
	parser.Prog(progName + " " + core->getName());
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });

	try
	{
		parser.ParseArgs(beginArgs, endArgs);
		if (beginArgs == endArgs) std::cout << parser;
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
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
	parser.Prog(argv[0]);
	parser.ProglinePostfix("{CORE OPTIONS}");

	args::Flag showCores(parser, "show-cores", "Shows all available system cores that can be used.", { "show-cores" });
	showCores.KickOut(true);

	args::Flag showAllCores(parser, "show-all-cores", "Shows all cores that can be currently loaded, including ones that are only loadable by other cores.", { "show-all-cores" });
	showAllCores.KickOut(true);

	args::MapPositional<std::string, Common::Core*> core(parser, "CORE", "Core to run.", cores);
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
