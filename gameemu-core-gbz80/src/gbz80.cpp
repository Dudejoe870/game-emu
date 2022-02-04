#include <game-emu/cores/processor/gbz80.h>

namespace GameEmu::Cores::Processor::GBZ80
{
	InstructionDecoder::Instruction* InstructionDecoder::getInstruction(const std::vector<unsigned long long>& opcodes)
	{
		if (opcodes.empty()) return nullptr;

		unsigned long long opcode = opcodes[0];
		Instruction* instructionTable = instructions.data();

		if (opcodes.size() > 1 && opcodes[0] == 0xCB)
		{
			if (opcodes[1] >= CBInstructions.size()) return nullptr;
			instructionTable = CBInstructions.data();
			opcode = opcodes[1];
		}
		else if (opcodes[0] >= instructions.size()) return nullptr;

		return &instructionTable[opcode];
	}

	std::string InstructionDecoder::Disassemble(const DecodeInfo& info)
	{
		Instruction* instruction = getInstruction(info.opcodes);
		if (!instruction) return "invalid instruction";
		return fmt::format(fmt::runtime(instruction->assemblyFormat),
			fmt::arg("d8", (info.operands.size() > 0) ? info.operands[0] : 0ull),
			fmt::arg("d16", (info.operands.size() > 1) ? 
				Common::Util::ToNativeEndian<std::endian::little>(
					*reinterpret_cast<const unsigned short*>(info.operands.data())) : 0ull)); // This optimization may seem a little over the top, but if you're disassembling a large chunk of code, this could add up.
	}

	InstructionDecoder::DecodeInfo InstructionDecoder::Decode(Common::InstructionStream& stream)
	{
		DecodeInfo info;

		unsigned long long firstOpcode = 0;
		if (!stream.getNext(firstOpcode)) return DecodeInfo();
		info.opcodes.push_back(firstOpcode);

		if (firstOpcode == 0xCB) // CB Prefix
		{
			unsigned long long secondOpcode = 0;
			if (!stream.getNext(secondOpcode)) return DecodeInfo();
			info.opcodes.push_back(secondOpcode);
		}

		info.instruction = getInstruction(info.opcodes);
		if (!info.instruction) return DecodeInfo();

		unsigned int operands = info.instruction->length - info.opcodes.size();
		for (unsigned int i = 0; i < operands; ++i)
		{
			unsigned long long operand = 0;
			if (!stream.getNext(operand)) return DecodeInfo();
			info.operands.push_back(operand);
		}

		return info;
	}

	Instance::Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties)
		: Common::CoreInstance(core, runState, properties), Common::InstructionBasedCoreInstance(decoder)
	{
	}

	Common::CoreInstance::ReturnStatus Instance::Step()
	{
		return ReturnStatus::Success;
	}

	std::string Instance::Disassemble(const std::vector<unsigned char>& data)
	{
		return DisassembleImpl<unsigned char, std::endian::little, true>(data);
	}

	Common::CoreState* Instance::getCoreState()
	{
		return &state;
	}

	Core::Core(Common::CoreLoader* loader)
		: Common::Core(loader)
	{
	}

	std::string Core::getName()
	{
		return "gbz80";
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
