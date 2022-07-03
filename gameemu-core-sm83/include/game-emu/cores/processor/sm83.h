#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/core.h>
#include <game-emu/common/instructionbasedcoreinstance.h>
#include <game-emu/common/coreloader.h>
#include <game-emu/common/runstate.h>

#include <game-emu/common/propertyvalue.h>

#include <game-emu/common/instructiondecoder.h>
#include <game-emu/common/instructionstream.h>

#include <game-emu/common/corestate.h>
#include <game-emu/common/register.h>

#include <game-emu/common/physicaladdresstranslator.h>

namespace GameEmu::Cores::Processor::SM83
{
	class Interpreter
	{
	public:
		static void NOP_CYC0(Common::CoreState* state, const std::vector<u64>& operands);
	};

	class InstructionStream : public Common::InstructionStream
	{
	private:
		u16 offset;
		Common::PhysicalAddressTranslator* memory;
		u16 PC;
	public:
		InstructionStream(u16 PC, Common::PhysicalAddressTranslator* memory)
		{
			this->PC = PC;
			this->offset = 0;
			this->memory = memory;
		}

		bool GetNext(u64& value)
		{
			value = memory->Read<u8, std::endian::native>(PC + offset++);
			return true;
		}
	};

	class InstructionDecoder : public Common::InstructionDecoder
	{
	private:
		std::vector<Instruction> instructions =
		{
			Instruction("nop", 1, { Interpreter::NOP_CYC0 }) // 0x00
		};

		std::vector<Instruction> CBInstructions =
		{

		};
	public:
		Instruction* GetInstruction(const std::vector<u64>& opcodes);
		std::string Disassemble(const DecodeInfo& info);
		DecodeInfo Decode(Common::InstructionStream& stream);
	};

	class Core : public Common::Core
	{
	public:
		Core(Common::CoreLoader* loader);

		std::string GetName();
		std::string GetDescription();
		Common::Core::Type GetType();
		std::unordered_map<std::string, Common::PropertyValue> GetDefaultProperties();
		
		std::shared_ptr<Common::CoreInstance> CreateNewInstance(
			Common::RunState& runState, std::unordered_map<std::string, Common::PropertyValue> properties = {});
	};

	class State : public Common::CoreState
	{
	public:
		struct RegisterState
		{
			// We store all the registers in little endian because the most used architectures are little endian, 
			// so on most architectures there won't be any extra instructions for byteswapping.
			union
			{
				u16 AF;
				struct
				{
					u8 F;
					u8 A;
				};
			};

			union
			{
				u16 BC;
				struct
				{
					u8 C;
					u8 B;
				};
			};

			union
			{
				u16 DE;
				struct
				{
					u8 E;
					u8 D;
				};
			};

			union
			{
				u16 HL;
				struct
				{
					u8 L;
					u8 H;
				};
			};

			u16 PC;

			RegisterState()
			{
				std::memset(this, 0, sizeof(RegisterState));
			}
		};

		RegisterState registerState;

		Common::Register<u16, std::endian::little, false> AF;
		Common::Register<u16, std::endian::little, false> BC;
		Common::Register<u16, std::endian::little, false> DE;
		Common::Register<u16, std::endian::little, false> HL;

		Common::Register<u8, std::endian::native, false> A;
		Common::Register<u8, std::endian::native, false> F;

		Common::Register<u8, std::endian::native, false> B;
		Common::Register<u8, std::endian::native, false> C;

		Common::Register<u8, std::endian::native, false> D;
		Common::Register<u8, std::endian::native, false> E;

		Common::Register<u8, std::endian::native, false> H;
		Common::Register<u8, std::endian::native, false> L;

		Common::Register<u16, std::endian::native, false> PC;

		Common::PhysicalAddressTranslator* idmem;

		u8 mCycles; // The amount of Machine Cycles took up by the last instruction.

		State()
			: AF(this, registerState.AF, registerState.AF, "AF", true, "{name}: 0x{u16:04x}"),
			BC(this, registerState.BC, registerState.BC, "BC", true, "{name}: 0x{u16:04x}"),
			DE(this, registerState.DE, registerState.DE, "DE", true, "{name}: 0x{u16:04x}"),
			HL(this, registerState.HL, registerState.HL, "HL", true, "{name}: 0x{u16:04x}"),
			A(this, registerState.A, registerState.A, "A", false),
			F(this, registerState.F, registerState.F, "F", false),
			B(this, registerState.B, registerState.B, "B", false),
			C(this, registerState.C, registerState.C, "C", false),
			D(this, registerState.D, registerState.D, "D", false),
			E(this, registerState.E, registerState.E, "E", false),
			H(this, registerState.E, registerState.E, "H", false),
			L(this, registerState.L, registerState.L, "L", false),
			PC(this, registerState.PC, registerState.PC, "PC", true, "{name}: 0x{u16:04x}")
		{
			this->idmem = nullptr;
			mCycles = 0;
		}
	};

	class Instance : public Common::InstructionBasedCoreInstance
	{
	private:
		InstructionDecoder decoder;

		State state;
		u8 cycleCounter;

		InstructionDecoder::DecodeInfo decodeInfo;

		inline void Fetch()
		{
			InstructionStream stream(state.PC, state.idmem);
			decodeInfo = decoder.Decode(stream);
		}

		std::chrono::nanoseconds stepPeriod;
	public:
		Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties);

		ReturnStatus Init();
		ReturnStatus Step();

		std::string Disassemble(const std::vector<u8>& data);
		Common::CoreState* GetCoreState();
		std::chrono::nanoseconds GetStepPeriod();
	};
}
