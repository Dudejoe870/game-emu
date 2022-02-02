#include <game-emu/common/instructionbasedcoreinstance.h>

#include <game-emu/common/instructionstream.h>

namespace GameEmu::Common
{
	InstructionBasedCoreInstance::InstructionBasedCoreInstance(InstructionDecoder& decoder)
		: decoder(decoder)
	{
	}
}
