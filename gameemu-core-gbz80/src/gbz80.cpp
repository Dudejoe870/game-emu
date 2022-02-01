#include <game-emu/cores/processor/gbz80.h>

namespace GameEmu::Cores::Processor::GBZ80
{
	InstructionDecoder::Instruction* InstructionDecoder::getInstruction(const std::vector<unsigned long long>& opcodes)
	{
		if (opcodes.size() <= 0 || opcodes[0] >= instructions.size()) return nullptr;
		return &instructions[opcodes[0]];
	}

	std::string InstructionDecoder::Disassemble(const DecodeInfo& info)
	{
		Instruction* instruction = getInstruction(info.opcodes);
		return instruction->assemblyFormat; // TODO: Format assembly from decoded info and instruction assembly format.
	}

	InstructionDecoder::DecodeInfo InstructionDecoder::Decode(Common::Cpu::InstructionStream& stream)
	{
		DecodeInfo info;

		// TODO: Decode Z80 instructions.

		return info;
	}

	CPU::ReturnStatus CPU::Step()
	{
		return ReturnStatus::Success;
	}

	Common::Cpu::InstructionDecoder* CPU::getInstructionDecoder()
	{
		return &decoder;
	}

	Instance::Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties)
		: Common::CoreInstance(core, runState, properties)
	{
	}

	Common::CoreInstance::ReturnStatus Instance::Step()
	{
		return ReturnStatus::Success;
	}

	Core::Core(Common::CoreLoader* loader)
		: Common::Core(loader)
	{
	}

	std::string Core::getName()
	{
		return "gameemu-core-gbz80";
	}

	std::string Core::getDescription()
	{
		return "A Core emulating a Gameboy's Z80 microprocessor.";
	}

	Common::Core::Type Core::getType()
	{
		return Common::Core::Type::Processor;
	}

	std::unique_ptr<Common::CoreInstance> Core::createNewInstance(Common::RunState& runState, std::unordered_map<std::string, Common::PropertyValue> properties)
	{
		return std::make_unique<Instance>(this, runState, properties);
	}
}
