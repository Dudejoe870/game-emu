#include <game-emu/cores/processor/sm83.h>

namespace GameEmu::Cores::Processor::SM83
{
	const InstructionDecoder::Instruction* InstructionDecoder::GetInstruction(const std::vector<u64>& opcodes)
	{
		if (opcodes.empty()) return nullptr;

		u64 opcode = opcodes[0];
		const Instruction* instructionTable = instructions.data();

		// If the opcode is prefixed with 0xCB, switch to the 0xCB Instruction table.
		if (opcodes.size() > 1 && opcodes[0] == 0xCB)
		{
			if (opcodes[1] >= CBInstructions.size()) return nullptr;
			instructionTable = CBInstructions.data();
			opcode = opcodes[1];
		}
		else if (opcodes[0] >= instructions.size()) return nullptr;

		const Instruction* inst = &instructionTable[opcode];
		if (inst->length == 0)
			return nullptr;
		return inst;
	}

	std::string InstructionDecoder::Disassemble(const DecodeInfo& info)
	{
		if (!info.instruction) return "illegal";
		return fmt::format(fmt::runtime(info.instruction->assemblyFormat),
			fmt::arg("d8", (info.operands.size() >= 1) ? info.operands[0] : 0ull),
			fmt::arg("ds8", (info.operands.size() >= 1) ? static_cast<s8>(info.operands[0]) : 0ull),
			fmt::arg("d16", (info.operands.size() >= 2) ? 
				Common::Util::ToNativeEndian<std::endian::little>(
					*reinterpret_cast<const u16*>(info.operands.data())) : 0ull));
	}

	InstructionDecoder::DecodeInfo InstructionDecoder::Decode(Common::InstructionStream& stream)
	{
		DecodeInfo info(2, 2);

		u64 firstOpcode = 0;
		if (!stream.GetNext(firstOpcode)) return DecodeInfo();
		info.opcodes.push_back(firstOpcode);

		if (firstOpcode == 0xCB) // CB Prefix
		{
			u64 secondOpcode = 0;
			if (!stream.GetNext(secondOpcode)) return DecodeInfo();
			info.opcodes.push_back(secondOpcode);
		}

		info.instruction = GetInstruction(info.opcodes);
		if (!info.instruction) return info;

		u32 operands = info.instruction->length - static_cast<u32>(info.opcodes.size());
		for (u32 i = 0; i < operands; ++i)
		{
			u64 operand = 0;
			if (!stream.GetNext(operand)) return DecodeInfo();
			info.operands.push_back(operand);
		}

		return info;
	}

	Instance::Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& propertyOverrides)
		: Common::InstructionBasedCoreInstance(core, runState, propertyOverrides, decoder)
	{
		stepPeriod = std::chrono::nanoseconds(1000000000U / static_cast<u64>(std::get<s64>(this->properties["freq"])));
		cycleCounter = 0;

		logger->LogInfo(fmt::format("SM83 initialized with frequency {}hz.",
			static_cast<u64>(std::get<s64>(this->properties["freq"]))));
	}

	Common::CoreInstance::ReturnStatus Instance::Init()
	{
		state.idmem = GetMemoryMap("idmem");

		Fetch();
		return ReturnStatus::Success;
	}

	Common::CoreInstance::ReturnStatus Instance::Step()
	{
		if (cycleCounter >= state.mCycles) cycleCounter = 0;

		if (!decodeInfo.instruction)
		{
			if (!decodeInfo.opcodes.empty())
			{
				if (decodeInfo.opcodes.size() <= 1)
					logger->LogError(fmt::format("SM83 Unknown Instruction 0x{:02x}",
						static_cast<u8>(decodeInfo.opcodes[0])));
				else
					logger->LogError(fmt::format("SM83 Unknown Instruction 0x{:02x}{:02x}",
						static_cast<u8>(decodeInfo.opcodes[0]),
						static_cast<u8>(decodeInfo.opcodes[1])));
			}
			return ReturnStatus::UnknownInstruction;
		}
		
		const InstructionDecoder::Instruction* inst = 
			reinterpret_cast<const InstructionDecoder::Instruction*>(decodeInfo.instruction);

		// Execute the Instruction
		if (cycleCounter < inst->interpreterCycles.size())
			inst->interpreterCycles[cycleCounter](state, decodeInfo.operands, decodeInfo.opcodes);
		
		// Fetch the next instruction if this is the last Cycle before the next execution.
		if (cycleCounter < state.mCycles && cycleCounter == state.mCycles - 1) Fetch();

		++cycleCounter;
		return ReturnStatus::Success;
	}

	std::string Instance::Disassemble(const std::vector<u8>& data)
	{
		return DisassembleImpl<u8, std::endian::little, true>(data);
	}
	
	const Common::CoreState& Instance::GetCoreState() const
	{
		return state;
	}

	std::chrono::nanoseconds Instance::GetStepPeriod() const
	{
		return stepPeriod;
	}

	Core::Core(Common::CoreLoader& loader)
		: Common::Core(loader)
	{
	}

	const std::string Core::GetName() const
	{
		return "sm83";
	}
	
	const std::string Core::GetDescription() const
	{
		return "A Core emulating a SM83 microprocessor.";
	}

	Common::Core::Type Core::GetType() const
	{
		return Common::Core::Type::Processor;
	}

	std::unordered_map<std::string, Common::PropertyValue> Core::GetDefaultProperties() const
	{
		return { { "freq", static_cast<s64>(1050000) } };
	}

	std::shared_ptr<Common::CoreInstance> Core::CreateNewInstance(
		Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& propertyOverrides)
	{
		return std::make_shared<Instance>(this, runState, propertyOverrides);
	}
}
