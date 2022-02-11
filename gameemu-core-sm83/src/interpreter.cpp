#include <game-emu/cores/processor/sm83.h>

namespace GameEmu::Cores::Processor::SM83
{
	void Interpreter::NOP_CYC0(Common::CoreState* state, const std::vector<u64>& operands)
	{
		State* z80State = static_cast<State*>(state);
		z80State->PC += 1;
		z80State->mCycles = 1;
	}
}
