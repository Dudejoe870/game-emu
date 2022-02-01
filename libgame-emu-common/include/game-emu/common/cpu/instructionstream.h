#pragma once

namespace GameEmu::Common::Cpu
{
	class InstructionStream
	{
	public:
		/*
		 Get next Instruction section (could be a byte stream like x86, could be a single full 32-bit instruction like mips)
		*/
		virtual unsigned long long getNext() = 0;
	};
}
