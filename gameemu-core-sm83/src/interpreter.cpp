#include <game-emu/cores/processor/sm83.h>

namespace GameEmu::Cores::Processor::SM83
{
	void Interpreter::NOP_CYC0(State& state, const std::vector<u64>& operands, const std::vector<u64>& opcodes)
	{
		state.PC += 1;
		state.mCycles = 1;
	}
}
