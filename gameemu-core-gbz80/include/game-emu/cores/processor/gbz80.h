#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/core.h>
#include <game-emu/common/coreinstance.h>
#include <game-emu/common/coreloader.h>
#include <game-emu/common/runstate.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/cpu/cpu.h>
#include <game-emu/common/cpu/instructiondecoder.h>
#include <game-emu/common/cpu/instructionstream.h>


namespace GameEmu::Cores::Processor::GBZ80
{
	class InstructionDecoder : public Common::Cpu::InstructionDecoder
	{
	private:
		std::vector<Instruction> instructions;
	public:
		Instruction* getInstruction(const std::vector<unsigned long long>& opcodes);
		std::string Disassemble(const DecodeInfo& info);
		DecodeInfo Decode(Common::Cpu::InstructionStream& stream);
	};

	class CPU : public Common::Cpu::CPU
	{
	private:
		InstructionDecoder decoder;
	public:
		ReturnStatus Step();
		Common::Cpu::InstructionDecoder* getInstructionDecoder();
	};

	class Instance : public Common::CoreInstance
	{
	private:
		CPU cpuCore;
	public:
		Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties);

		ReturnStatus Step();
	};

	class Core : public Common::Core
	{
	public:
		Core(Common::CoreLoader* loader);
		
		std::string getName();
		std::string getDescription();
		Common::Core::Type getType();
		std::unique_ptr<Common::CoreInstance> createNewInstance(Common::RunState& runState, std::unordered_map<std::string, Common::PropertyValue> properties = {});
	};
}
