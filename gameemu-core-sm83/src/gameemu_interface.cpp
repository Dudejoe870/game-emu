#include <game-emu/cores/processor/sm83.h>

#include <game-emu/common/symbolhelper.h>
#include <game-emu/common/coreloader.h>

extern "C" GAMEEMU_INTERFACE_EXPORT GameEmu::Common::Core* GameEmuInitCore(GameEmu::Common::CoreLoader* loader)
{
	return new GameEmu::Cores::Processor::SM83::Core(loader);
}
