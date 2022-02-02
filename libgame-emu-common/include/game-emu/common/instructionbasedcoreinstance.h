#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/coreinstance.h>

#include <game-emu/common/instructiondecoder.h>

namespace GameEmu::Common
{
	class InstructionBasedCoreInstance : virtual public CoreInstance
	{
	private:
		InstructionDecoder& decoder;
	protected:
		template <class T, std::endian endian>
		inline std::string DisassembleImpl(const std::vector<unsigned char>& data)
		{
			std::string result = "";

			unsigned int currentOffset = 0;
			while (currentOffset < (unsigned int)data.size())
			{
				Common::VectorInstructionStream<T, endian> instructionStream(data, currentOffset);
				InstructionDecoder::DecodeInfo decodeInfo = decoder.Decode(instructionStream);
				InstructionDecoder::Instruction* instruction = decoder.getInstruction(decodeInfo.operands);

				result += decoder.Disassemble(decodeInfo) + '\n';

				currentOffset += instruction->length;
			}

			return result;
		}
	public:
		LIBGAMEEMU_COMMON_DLL_EXPORT InstructionBasedCoreInstance(InstructionDecoder& decoder);

		/*
		 Returns a disassembled string with each instruction separated by a new line.
		*/
		virtual std::string Disassemble(const std::vector<unsigned char>& data) = 0;
	};
}
