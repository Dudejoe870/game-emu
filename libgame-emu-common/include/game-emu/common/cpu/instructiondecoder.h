#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/cpu/instructionstream.h>

namespace GameEmu::Common::Cpu
{
	class InstructionDecoder
	{
	public:
		struct DecodeInfo
		{
			std::vector<unsigned long long> opcodes; // Could be just one value, or multiple, it's up the the implementation.
			std::vector<unsigned long long> operands;

			unsigned char instructionLength;
		};

		struct Instruction
		{
			/*
			 A format string representing this instruction in assembly language.
			*/
			std::string assemblyFormat;

			Instruction(std::string assemblyFormat)
			{
				this->assemblyFormat = assemblyFormat;
			}
		};

		/*
		 Given the opcode(s), get a pointer to the instruction information (probably stored in a list that is indexed by the opcodes in some way)
		*/
		virtual Instruction* getInstruction(const std::vector<unsigned long long>& opcodes) = 0;

		/*
		 Given a decoded instruction, using the instructions stored assembly format, return the formatted assembly instruction (for debugging purposes)
		*/
		virtual std::string Disassemble(const DecodeInfo& info) = 0;

		/*
		 Decodes the Instruction held in the stream.

		 The stream could just be one integer of any size, 
		 or could be a byte stream of multiple bytes making up a single instruction, 
		 it depends on the implementation. No matter what though, this function decodes only one instruction at a time.
		*/
		virtual DecodeInfo Decode(InstructionStream& stream) = 0;
	};
}
