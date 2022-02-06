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

namespace GameEmu::Cores::Processor::GBZ80
{
	class InstructionDecoder : public Common::InstructionDecoder
	{
	private:
		std::vector<Instruction> instructions = 
		{
			Instruction("nop", 1) // 0x00
		};

		std::vector<Instruction> CBInstructions =
		{

		};
	public:
		Instruction* getInstruction(const std::vector<u64>& opcodes);
		std::string Disassemble(const DecodeInfo& info);
		DecodeInfo Decode(Common::InstructionStream& stream);
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

		State()
			: AF(this, registerState.AF, registerState.AF, "AF"),
			  BC(this, registerState.BC, registerState.BC, "BC"),
			  DE(this, registerState.DE, registerState.DE, "DE"),
			  HL(this, registerState.HL, registerState.HL, "HL"),
			  A(this, registerState.A, registerState.A, "A"),
			  F(this, registerState.F, registerState.F, "F"),
			  B(this, registerState.B, registerState.B, "B"),
			  C(this, registerState.C, registerState.C, "C"),
			  D(this, registerState.D, registerState.D, "D"),
			  E(this, registerState.E, registerState.E, "E"),
			  H(this, registerState.E, registerState.E, "H"),
			  L(this, registerState.L, registerState.L, "L")
		{
		}
	};

	class Instance : public Common::InstructionBasedCoreInstance
	{
	private:
		InstructionDecoder decoder;

		State state;
	public:
		Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& properties);

		ReturnStatus Step();

		std::string Disassemble(const std::vector<u8>& data);
		Common::CoreState* getCoreState();
		std::chrono::nanoseconds getStepPeriod();
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
