#include <game-emu/cores/system/gb.h>

#include <game-emu/common/symbolhelper.h>
#include <game-emu/common/coreloader.h>

extern "C" GAMEEMU_INTERFACE_EXPORT GameEmu::Common::Core* GameEmuInitCore(GameEmu::Common::CoreLoader& loader)
{
	return new GameEmu::Cores::System::GB::Core(loader);
}
