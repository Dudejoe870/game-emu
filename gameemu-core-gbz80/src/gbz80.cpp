#include <game-emu/cores/processor/gbz80.h>

namespace GameEmu::Cores::Processor::GBZ80
{
	InstructionDecoder::Instruction* InstructionDecoder::getInstruction(const std::vector<u64>& opcodes)
	{
		if (opcodes.empty()) return nullptr;

		u64 opcode = opcodes[0];
		Instruction* instructionTable = instructions.data();

		// If the opcode is prefixed with 0xCB, switch to the 0xCB Instruction table.
		if (opcodes.size() > 1 && opcodes[0] == 0xCB)
		{
			if (opcodes[1] >= CBInstructions.size()) return nullptr;
			instructionTable = CBInstructions.data();
			opcode = opcodes[1];
		}
		else if (opcodes[0] >= instructions.size()) return nullptr;

		Instruction* inst = &instructionTable[opcode];
		if (!inst->set) return nullptr;
		return inst;
	}

	std::string InstructionDecoder::Disassemble(const DecodeInfo& info)
	{
		if (!info.instruction) return "invalid instruction";
		return fmt::format(fmt::runtime(info.instruction->assemblyFormat),
			fmt::arg("d8", (info.operands.size() > 0) ? info.operands[0] : 0ull),
			fmt::arg("d16", (info.operands.size() > 1) ? 
				Common::Util::ToNativeEndian<std::endian::little>(
					*reinterpret_cast<const u16*>(info.operands.data())) : 0ull)); // This optimization may seem a little over the top, but if you're disassembling a large chunk of code, this could add up.
	}

	InstructionDecoder::DecodeInfo InstructionDecoder::Decode(Common::InstructionStream& stream)
	{
		DecodeInfo info;

		u64 firstOpcode = 0;
		if (!stream.getNext(firstOpcode)) return DecodeInfo();
		info.opcodes.push_back(firstOpcode);

		if (firstOpcode == 0xCB) // CB Prefix
		{
			u64 secondOpcode = 0;
			if (!stream.getNext(secondOpcode)) return DecodeInfo();
			info.opcodes.push_back(secondOpcode);
		}

		info.instruction = getInstruction(info.opcodes);
		if (!info.instruction) return DecodeInfo();

		u32 operands = info.instruction->length - (u32)info.opcodes.size();
		for (u32 i = 0; i < operands; ++i)
		{
			u64 operand = 0;
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

	std::string Instance::Disassemble(const std::vector<u8>& data)
	{
		return DisassembleImpl<u8, std::endian::little, true>(data);
	}

	Common::CoreState* Instance::getCoreState()
	{
		return &state;
	}

	std::chrono::nanoseconds Instance::getStepPeriod()
	{
		return std::chrono::nanoseconds(952); // 1.05 MHz
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
