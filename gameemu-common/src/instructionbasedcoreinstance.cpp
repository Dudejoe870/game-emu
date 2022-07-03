#include <game-emu/common/instructionbasedcoreinstance.h>

#include <game-emu/common/instructionstream.h>

namespace GameEmu::Common
{
	InstructionBasedCoreInstance::InstructionBasedCoreInstance(Core* core, RunState& runState,
		const std::unordered_map<std::string, PropertyValue>& properties, InstructionDecoder& decoder)
		: CoreInstance(core, runState, properties), decoder(decoder)
	{
	}
}
