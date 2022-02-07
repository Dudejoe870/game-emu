#include <game-emu/cores/processor/gbz80.h>

namespace GameEmu::Cores::Processor::GBZ80
{
	void Interpreter::NOP(Common::CoreState* state, const std::vector<u64>& operands)
	{
		State* z80State = reinterpret_cast<State*>(state);
		z80State->PC += 1;
	}
}
