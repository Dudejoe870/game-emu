#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/instructionstream.h>

#include <game-emu/common/coreinstance.h>
#include <game-emu/common/corestate.h>

namespace GameEmu::Common
{
	class InstructionDecoder
	{
	public:
		struct Instruction
		{
			/*
			 A format string representing this instruction in assembly language.
			*/
			std::string assemblyFormat;

			/*
			 The length of this instruction in bytes.
			*/
			u8 length;

			Instruction(std::string assemblyFormat, unsigned char length)
			{
				this->assemblyFormat = assemblyFormat;
				this->length = length;
			}
		};

		struct DecodeInfo
		{
			std::vector<u64> opcodes; // Could be just one value, or multiple, it's up the the implementation.
			std::vector<u64> operands;
			const Instruction* instruction;

			DecodeInfo()
			{
				this->instruction = nullptr;
			}
			
			DecodeInfo(std::size_t opcodesCapacity, std::size_t operandsCapacity)
			{
				opcodes.reserve(opcodesCapacity);
				operands.reserve(operandsCapacity);
				
				this->instruction = nullptr;
			}

			DecodeInfo(const std::vector<u64>& opcodes, const std::vector<u64>& operands, const Instruction* instruction)
			{
				this->opcodes = opcodes;
				this->operands = operands;
				this->instruction = instruction;
			}
		};

		/*
		 Given the opcode(s), get a pointer to the instruction information (probably stored in a list that is indexed by the opcodes in some way)
		 Should return nullptr if the instruction is invalid (doesn't exist or the "set" boolean is false)
		*/
		virtual const Instruction* GetInstruction(const std::vector<u64>& opcodes) = 0;

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
