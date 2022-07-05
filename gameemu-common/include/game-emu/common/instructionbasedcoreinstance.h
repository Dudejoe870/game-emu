#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/symbolhelper.h>

#include <game-emu/common/coreinstance.h>

#include <game-emu/common/instructiondecoder.h>

namespace GameEmu::Common
{
	class InstructionBasedCoreInstance : public CoreInstance
	{
	private:
		InstructionDecoder& decoder;
	protected:
		/*
		 General Disassembly Implementation.
		 Note: It's recommended to set stopUnknownInst if the emulated ISA has variable length instructions.
		*/
		template <class T, std::endian endian, bool stopUnknownInst = false, unsigned int defaultInstructionSize = 1>
		inline std::string DisassembleImpl(const std::vector<unsigned char>& data)
		{
			std::string result = "";

			unsigned int currentOffset = 0;
			while (currentOffset < (unsigned int)data.size())
			{
				Common::VectorInstructionStream<T, endian> instructionStream(data, currentOffset);
				InstructionDecoder::DecodeInfo decodeInfo = decoder.Decode(instructionStream);

				result += decoder.Disassemble(decodeInfo) + '\n';

				u8 instructionLength = std::max(u8(1), decodeInfo.instruction->length);
				if constexpr (stopUnknownInst)
				{
					if (!decodeInfo.instruction) break;
					currentOffset += instructionLength;
				}
				else currentOffset += (decodeInfo.instruction) ? instructionLength : defaultInstructionSize;
			}

			return result;
		}
	public:
		LIBGAMEEMU_COMMON_DLL_EXPORT InstructionBasedCoreInstance(Core* core, RunState& runState,
			const std::unordered_map<std::string, PropertyValue>& properties, InstructionDecoder& decoder);

		/*
		 Returns a disassembled string with each instruction separated by a new line.
		*/
		virtual std::string Disassemble(const std::vector<u8>& data) = 0;
	};
}
