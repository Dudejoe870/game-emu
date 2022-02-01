#include <game-emu/common/cpu/cpu.h>

namespace GameEmu::Common::Cpu
{
	CPU::ReturnStatus CPU::Step()
	{
		return ReturnStatus::Error;
	}

	InstructionDecoder* CPU::getInstructionDecoder()
	{
		return nullptr;
	}
}
