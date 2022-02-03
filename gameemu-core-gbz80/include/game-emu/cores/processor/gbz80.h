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
		Instruction* getInstruction(const std::vector<unsigned long long>& opcodes);
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
				unsigned short AF;
				struct
				{
					unsigned char F;
					unsigned char A;
				};
			};

			union
			{
				unsigned short BC;
				struct
				{
					unsigned char C;
					unsigned char B;
				};
			};

			union
			{
				unsigned short DE;
				struct
				{
					unsigned char E;
					unsigned char D;
				};
			};

			union
			{
				unsigned short HL;
				struct
				{
					unsigned char L;
					unsigned char H;
				};
			};

			RegisterState()
			{
				std::memset(this, 0, sizeof(RegisterState));
			}
		};

		RegisterState registerState;

		Common::Register<unsigned short, std::endian::little, false> AF;
		Common::Register<unsigned short, std::endian::little, false> BC;
		Common::Register<unsigned short, std::endian::little, false> DE;
		Common::Register<unsigned short, std::endian::little, false> HL;

		Common::Register<unsigned char, std::endian::native, false> A;
		Common::Register<unsigned char, std::endian::native, false> F;

		Common::Register<unsigned char, std::endian::native, false> B;
		Common::Register<unsigned char, std::endian::native, false> C;

		Common::Register<unsigned char, std::endian::native, false> D;
		Common::Register<unsigned char, std::endian::native, false> E;

		Common::Register<unsigned char, std::endian::native, false> H;
		Common::Register<unsigned char, std::endian::native, false> L;
	public:
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

		std::string Disassemble(const std::vector<unsigned char>& data);
		Common::CoreState* getCoreState();
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
