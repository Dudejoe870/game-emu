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

#include <game-emu/common/physicalmemorymap.h>

namespace GameEmu::Cores::Processor::SM83
{
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

		std::shared_ptr<Common::PhysicalMemoryMap> idmem;

		u8 mCycles; // The amount of Machine Cycles taken up by the current instruction.

		State()
			: AF(*this, registerState.AF, registerState.AF, "AF", true, "{name}: 0x{u16:04x}"),
			BC(*this, registerState.BC, registerState.BC, "BC", true, "{name}: 0x{u16:04x}"),
			DE(*this, registerState.DE, registerState.DE, "DE", true, "{name}: 0x{u16:04x}"),
			HL(*this, registerState.HL, registerState.HL, "HL", true, "{name}: 0x{u16:04x}"),
			A(*this, registerState.A, registerState.A, "A", false),
			F(*this, registerState.F, registerState.F, "F", false),
			B(*this, registerState.B, registerState.B, "B", false),
			C(*this, registerState.C, registerState.C, "C", false),
			D(*this, registerState.D, registerState.D, "D", false),
			E(*this, registerState.E, registerState.E, "E", false),
			H(*this, registerState.H, registerState.H, "H", false),
			L(*this, registerState.L, registerState.L, "L", false),
			PC(*this, registerState.PC, registerState.PC, "PC", true, "{name}: 0x{u16:04x}")
		{
			this->idmem = nullptr;
			mCycles = 0;
		}
	};

	class Interpreter
	{
	public:
		static void NOP_CYC0(State& state, const std::vector<u64>& operands, const std::vector<u64>& opcodes);
	};

	class InstructionStream : public Common::InstructionStream
	{
	private:
		u16 offset;
		std::shared_ptr<Common::PhysicalMemoryMap> memory;
		u16 PC;
	public:
		InstructionStream(u16 PC, const std::shared_ptr<Common::PhysicalMemoryMap>& memory)
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
	public:
		struct Instruction : public Common::InstructionDecoder::Instruction
		{
			using InterpreterFunction = std::function<void(State&, const std::vector<u64>&, const std::vector<u64>&)>;
			std::vector<InterpreterFunction> interpreterCycles;

			Instruction(std::string assemblyFormat, unsigned char length, const std::vector<InterpreterFunction>& interpreterCycles)
				: Common::InstructionDecoder::Instruction(assemblyFormat, length)
			{
				this->interpreterCycles = interpreterCycles;
			}
		};
	private:
		const std::vector<Instruction> instructions =
		{
			Instruction("nop", 1, { Interpreter::NOP_CYC0 }), // 0x00
			Instruction("ld BC, 0x{d16:04x}", 3, { }), // 0x01
			Instruction("ld (BC), A", 1, { }), // 0x02
			Instruction("inc BC", 1, { }), // 0x03
			Instruction("inc B", 1, { }), // 0x04
			Instruction("dec B", 1, { }), // 0x05
			Instruction("ld B, 0x{d8:02x}", 2, { }), // 0x06
			Instruction("rlca", 1, { }), // 0x07
			Instruction("ld (0x{d16:04x}), SP", 3, { }), // 0x08
			Instruction("add HL, BC", 1, { }), // 0x09
			Instruction("ld A, (BC)", 1, { }), // 0x0A
			Instruction("dec BC", 1, { }), // 0x0B
			Instruction("inc C", 1, { }), // 0x0C
			Instruction("dec C", 1, { }), // 0x0D
			Instruction("ld C, 0x{d8:02x}", 2, { }), // 0x0E
			Instruction("rrca", 1, { }), // 0x0F
			Instruction("stop", 1, { }), // 0x10
			Instruction("ld DE, 0x{d16:04x}", 3, { }), // 0x11
			Instruction("ld (DE), A", 1, { }), // 0x12
			Instruction("inc DE", 1, { }), // 0x13
			Instruction("inc D", 1, { }), // 0x14
			Instruction("dec D", 1, { }), // 0x15
			Instruction("ld D, 0x{d8:02x}", 2, { }), // 0x16
			Instruction("rla", 1, { }), // 0x17
			Instruction("jr 0x{d8:02x}", 2, { }), // 0x18
			Instruction("add HL, DE", 1, { }), // 0x19
			Instruction("ld A, (DE)", 1, { }), // 0x1A
			Instruction("dec DE", 1, { }), // 0x1B
			Instruction("inc E", 1, { }), // 0x1C
			Instruction("dec E", 1, { }), // 0x1D
			Instruction("ld E, 0x{d8:02x}", 2, { }), // 0x1E
			Instruction("rra", 1, { }), // 0x1F
			Instruction("jr nz, 0x{d8:02x}", 2, { }), // 0x20
			Instruction("ld HL, 0x{d16:04x}", 3, { }), // 0x21
			Instruction("ld (HL+), A", 1, { }), // 0x22
			Instruction("inc HL", 1, { }), // 0x23
			Instruction("inc H", 1, { }), // 0x24
			Instruction("dec H", 1, { }), // 0x25
			Instruction("ld H, 0x{d8:02x}", 2, { }), // 0x26
			Instruction("daa", 1, { }), // 0x27
			Instruction("jr z, 0x{d8:02x}", 2, { }), // 0x28
			Instruction("add HL, HL", 1, { }), // 0x29
			Instruction("ld A, (HL+)", 1, { }), // 0x2A
			Instruction("dec HL", 1, { }), // 0x2B
			Instruction("inc L", 1, { }), // 0x2C
			Instruction("dec L", 1, { }), // 0x2D
			Instruction("ld L, 0x{d8:02x}", 2, { }), // 0x2E
			Instruction("cpl", 1, { }), // 0x2F
			Instruction("jr nc, 0x{d8:02x}", 2, { }), // 0x30
			Instruction("ld SP, 0x{d16:04x}", 3, { }), // 0x31
			Instruction("ld (HL-), A", 1, { }), // 0x32
			Instruction("inc SP", 1, { }), // 0x33
			Instruction("inc (HL)", 1, { }), // 0x34
			Instruction("dec (HL)", 1, { }), // 0x35
			Instruction("ld (HL), 0x{d8:02x}", 2, { }), // 0x36
			Instruction("scf", 1, { }), // 0x37
			Instruction("jr c, 0x{d8:02x}", 2, { }), // 0x38
			Instruction("add HL, SP", 1, { }), // 0x39
			Instruction("ld A, (HL-)", 1, { }), // 0x3A
			Instruction("dec SP", 1, { }), // 0x3B
			Instruction("inc A", 1, { }), // 0x3C
			Instruction("dec A", 1, { }), // 0x3D
			Instruction("ld A, 0x{d8:02x}", 2, { }), // 0x3E
			Instruction("ccf", 1, { }), // 0x3F
			Instruction("ld B, B", 1, { }), // 0x40
			Instruction("ld B, C", 1, { }), // 0x41
			Instruction("ld B, D", 1, { }), // 0x42
			Instruction("ld B, E", 1, { }), // 0x43
			Instruction("ld B, H", 1, { }), // 0x44
			Instruction("ld B, L", 1, { }), // 0x45
			Instruction("ld B, (HL)", 1, { }), // 0x46
			Instruction("ld B, A", 1, { }), // 0x47
			Instruction("ld C, B", 1, { }), // 0x48
			Instruction("ld C, C", 1, { }), // 0x49
			Instruction("ld C, D", 1, { }), // 0x4A
			Instruction("ld C, E", 1, { }), // 0x4B
			Instruction("ld C, H", 1, { }), // 0x4C
			Instruction("ld C, L", 1, { }), // 0x4D
			Instruction("ld C, (HL)", 1, { }), // 0x4E
			Instruction("ld C, A", 1, { }), // 0x4F
			Instruction("ld D, B", 1, { }), // 0x50
			Instruction("ld D, C", 1, { }), // 0x51
			Instruction("ld D, D", 1, { }), // 0x52
			Instruction("ld D, E", 1, { }), // 0x53
			Instruction("ld D, H", 1, { }), // 0x54
			Instruction("ld D, L", 1, { }), // 0x55
			Instruction("ld D, (HL)", 1, { }), // 0x56
			Instruction("ld D, A", 1, { }), // 0x57
			Instruction("ld E, B", 1, { }), // 0x58
			Instruction("ld E, C", 1, { }), // 0x59
			Instruction("ld E, D", 1, { }), // 0x5A
			Instruction("ld E, E", 1, { }), // 0x5B
			Instruction("ld E, H", 1, { }), // 0x5C
			Instruction("ld E, L", 1, { }), // 0x5D
			Instruction("ld E, (HL)", 1, { }), // 0x5E
			Instruction("ld E, A", 1, { }), // 0x5F
			Instruction("ld H, B", 1, { }), // 0x60
			Instruction("ld H, C", 1, { }), // 0x61
			Instruction("ld H, D", 1, { }), // 0x62
			Instruction("ld H, E", 1, { }), // 0x63
			Instruction("ld H, H", 1, { }), // 0x64
			Instruction("ld H, L", 1, { }), // 0x65
			Instruction("ld H, (HL)", 1, { }), // 0x66
			Instruction("ld H, A", 1, { }), // 0x67
			Instruction("ld L, B", 1, { }), // 0x68
			Instruction("ld L, C", 1, { }), // 0x69
			Instruction("ld L, D", 1, { }), // 0x6A
			Instruction("ld L, E", 1, { }), // 0x6B
			Instruction("ld L, H", 1, { }), // 0x6C
			Instruction("ld L, L", 1, { }), // 0x6D
			Instruction("ld L, (HL)", 1, { }), // 0x6E
			Instruction("ld L, A", 1, { }), // 0x6F
			Instruction("ld (HL), B", 1, { }), // 0x70
			Instruction("ld (HL), C", 1, { }), // 0x71
			Instruction("ld (HL), D", 1, { }), // 0x72
			Instruction("ld (HL), E", 1, { }), // 0x73
			Instruction("ld (HL), H", 1, { }), // 0x74
			Instruction("ld (HL), L", 1, { }), // 0x75
			Instruction("halt", 1, { }), // 0x76
			Instruction("ld (HL), A", 1, { }), // 0x77
			Instruction("ld A, B", 1, { }), // 0x78
			Instruction("ld A, C", 1, { }), // 0x79
			Instruction("ld A, D", 1, { }), // 0x7A
			Instruction("ld A, E", 1, { }), // 0x7B
			Instruction("ld A, H", 1, { }), // 0x7C
			Instruction("ld A, L", 1, { }), // 0x7D
			Instruction("ld A, (HL)", 1, { }), // 0x7E
			Instruction("ld A, A", 1, { }), // 0x7F
			Instruction("add A, B", 1, { }), // 0x80
			Instruction("add A, C", 1, { }), // 0x81
			Instruction("add A, D", 1, { }), // 0x82
			Instruction("add A, E", 1, { }), // 0x83
			Instruction("add A, H", 1, { }), // 0x84
			Instruction("add A, L", 1, { }), // 0x85
			Instruction("add A, (HL)", 1, { }), // 0x86
			Instruction("add A, A", 1, { }), // 0x87
			Instruction("adc A, B", 1, { }), // 0x88
			Instruction("adc A, C", 1, { }), // 0x89
			Instruction("adc A, D", 1, { }), // 0x8A
			Instruction("adc A, E", 1, { }), // 0x8B
			Instruction("adc A, H", 1, { }), // 0x8C
			Instruction("adc A, L", 1, { }), // 0x8D
			Instruction("adc A, (HL)", 1, { }), // 0x8E
			Instruction("adc A, A", 1, { }), // 0x8F
			Instruction("sub B", 1, { }), // 0x90
			Instruction("sub C", 1, { }), // 0x91
			Instruction("sub D", 1, { }), // 0x92
			Instruction("sub E", 1, { }), // 0x93
			Instruction("sub H", 1, { }), // 0x94
			Instruction("sub L", 1, { }), // 0x95
			Instruction("sub (HL)", 1, { }), // 0x96
			Instruction("sub A", 1, { }), // 0x97
			Instruction("sbc A, B", 1, { }), // 0x98
			Instruction("sbc A, C", 1, { }), // 0x99
			Instruction("sbc A, D", 1, { }), // 0x9A
			Instruction("sbc A, E", 1, { }), // 0x9B
			Instruction("sbc A, H", 1, { }), // 0x9C
			Instruction("sbc A, L", 1, { }), // 0x9D
			Instruction("sbc A, (HL)", 1, { }), // 0x9E
			Instruction("sbc A, A", 1, { }), // 0x9F
			Instruction("and B", 1, { }), // 0xA0
			Instruction("and C", 1, { }), // 0xA1
			Instruction("and D", 1, { }), // 0xA2
			Instruction("and E", 1, { }), // 0xA3
			Instruction("and H", 1, { }), // 0xA4
			Instruction("and L", 1, { }), // 0xA5
			Instruction("and (HL)", 1, { }), // 0xA6
			Instruction("and A", 1, { }), // 0xA7
			Instruction("xor B", 1, { }), // 0xA8
			Instruction("xor C", 1, { }), // 0xA9
			Instruction("xor D", 1, { }), // 0xAA
			Instruction("xor E", 1, { }), // 0xAB
			Instruction("xor H", 1, { }), // 0xAC
			Instruction("xor L", 1, { }), // 0xAD
			Instruction("xor (HL)", 1, { }), // 0xAE
			Instruction("xor A", 1, { }), // 0xAF
			Instruction("or B", 1, { }), // 0xB0
			Instruction("or C", 1, { }), // 0xB1
			Instruction("or D", 1, { }), // 0xB2
			Instruction("or E", 1, { }), // 0xB3
			Instruction("or H", 1, { }), // 0xB4
			Instruction("or L", 1, { }), // 0xB5
			Instruction("or (HL)", 1, { }), // 0xB6
			Instruction("or A", 1, { }), // 0xB7
			Instruction("cp B", 1, { }), // 0xB8
			Instruction("cp C", 1, { }), // 0xB9
			Instruction("cp D", 1, { }), // 0xBA
			Instruction("cp E", 1, { }), // 0xBB
			Instruction("cp H", 1, { }), // 0xBC
			Instruction("cp L", 1, { }), // 0xBD
			Instruction("cp (HL)", 1, { }), // 0xBE
			Instruction("cp A", 1, { }), // 0xBF
			Instruction("ret nz", 1, { }), // 0xC0
			Instruction("pop bc", 1, { }), // 0xC1
			Instruction("jp NZ, 0x{d16:04x}", 3, { }), // 0xC2
			Instruction("jp 0x{d16:04x}", 3, { }), // 0xC3
			Instruction("call NZ, 0x{d16:04x}", 3, { }), // 0xC4
			Instruction("push BC", 1, { }), // 0xC5
			Instruction("add A, 0x{d8:02x}", 2, { }), // 0xC6
			Instruction("rst 0x00", 1, { }), // 0xC7
			Instruction("ret Z", 1, { }), // 0xC8
			Instruction("ret", 1, { }), // 0xC9
			Instruction("jp Z, 0x{d16:04x}", 3, { }), // 0xCA
			Instruction("", 0, { }), // 0xCB
			Instruction("call Z, 0x{d16:04x}", 3, { }), // 0xCC
			Instruction("call 0x{d16:04x}", 3, { }), // 0xCD
			Instruction("adc A, 0x{d8:02x}", 2, { }), // 0xCE
			Instruction("rst 0x08", 1, { }), // 0xCF
			Instruction("ret NC", 1, { }), // 0xD0
			Instruction("pop DE", 1, { }), // 0xD1
			Instruction("jp NC, 0x{d16:02x}", 3, { }), // 0xD2
			Instruction("illegal", 0, { }), // 0xD3
			Instruction("call NC, 0x{d16:04x}", 3, { }), // 0xD4
			Instruction("push DE", 1, { }), // 0xD5
			Instruction("sub 0x{d8:02x}", 2, { }), // 0xD6
			Instruction("rst 0x10", 1, { }), // 0xD7
			Instruction("ret C", 1, { }), // 0xD8
			Instruction("reti", 1, { }), // 0xD9
			Instruction("jp C, 0x{d16:04x}", 3, { }), // 0xDA
			Instruction("illegal", 0, { }), // 0xDB
			Instruction("call C, 0x{d16:04x}", 3, { }), // 0xDC
			Instruction("illegal", 0, { }), // 0xDD
			Instruction("sbc A, 0x{d8:02x}", 2, { }), // 0xDE
			Instruction("rst 0x18", 1, { }), // 0xDF
			Instruction("ld (0xFF{d8:02x}), A", 2, { }), // 0xE0
			Instruction("pop HL", 1, { }), // 0xE1
			Instruction("ld (C), A", 1, { }), // 0xE2
			Instruction("illegal", 0, { }), // 0xE3
			Instruction("illegal", 0, { }), // 0xE4
			Instruction("push HL", 1, { }), // 0xE5
			Instruction("and 0x{d8:02x}", 2, { }), // 0xE6
			Instruction("rst 0x20", 1, { }), // 0xE7
			Instruction("add SP, 0x{d8:02x}", 2, { }), // 0xE8
			Instruction("jp (HL)", 1, { }), // 0xE9
			Instruction("ld (0x{d16:04x}), A", 3, { }), // 0xEA
			Instruction("illegal", 0, { }), // 0xEB
			Instruction("illegal", 0, { }), // 0xEC
			Instruction("illegal", 0, { }), // 0xED
			Instruction("xor 0x{d8:02x}", 2, { }), // 0xEE
			Instruction("rst 0x28", 1, { }), // 0xEF
			Instruction("ld A, (0xFF{d8:02x})", 2, { }), // 0xF0
			Instruction("pop AF", 1, { }), // 0xF1
			Instruction("ld A, (C)", 1, { }), // 0xF2
			Instruction("di", 1, { }), // 0xF3
			Instruction("illegal", 0, { }), // 0xF4
			Instruction("push AF", 1, { }), // 0xF5
			Instruction("or 0x{d8:02x}", 2, { }), // 0xF6
			Instruction("rst 0x30", 1, { }), // 0xF7
			Instruction("ld HL, SP + {ds8}", 2, { }), // 0xF8
			Instruction("ld SP, HL", 1, { }), // 0xF9
			Instruction("ld A, (0x{d16:04x})", 3, { }), // 0xFA
			Instruction("ei", 1, { }), // 0xFB
			Instruction("illegal", 0, { }), // 0xFC
			Instruction("illegal", 0, { }), // 0xFD
			Instruction("cp 0x{d8:02x}", 2, { }), // 0xFE
			Instruction("rst 0x38", 1, { }), // 0xFF
		};
		
		const std::vector<Instruction> CBInstructions =
		{
			Instruction("rlc B", 2, { }), // 0x00
			Instruction("rlc C", 2, { }), // 0x01
			Instruction("rlc D", 2, { }), // 0x02
			Instruction("rlc E", 2, { }), // 0x03
			Instruction("rlc H", 2, { }), // 0x04
			Instruction("rlc L", 2, { }), // 0x05
			Instruction("rlc (HL)", 2, { }), // 0x06
			Instruction("rlc A", 2, { }), // 0x07
			Instruction("rrc B", 2, { }), // 0x08
			Instruction("rrc C", 2, { }), // 0x09
			Instruction("rrc D", 2, { }), // 0x0A
			Instruction("rrc E", 2, { }), // 0x0B
			Instruction("rrc H", 2, { }), // 0x0C
			Instruction("rrc L", 2, { }), // 0x0D
			Instruction("rrc (HL)", 2, { }), // 0x0E
			Instruction("rrc A", 2, { }), // 0x0F
			Instruction("rl B", 2, { }), // 0x10
			Instruction("rl C", 2, { }), // 0x11
			Instruction("rl D", 2, { }), // 0x12
			Instruction("rl E", 2, { }), // 0x13
			Instruction("rl H", 2, { }), // 0x14
			Instruction("rl L", 2, { }), // 0x15
			Instruction("rl (HL)", 2, { }), // 0x16
			Instruction("rl A", 2, { }), // 0x17
			Instruction("rr B", 2, { }), // 0x18
			Instruction("rr C", 2, { }), // 0x19
			Instruction("rr D", 2, { }), // 0x1A
			Instruction("rr E", 2, { }), // 0x1B
			Instruction("rr H", 2, { }), // 0x1C
			Instruction("rr L", 2, { }), // 0x1D
			Instruction("rr (HL)", 2, { }), // 0x1E
			Instruction("rr A", 2, { }), // 0x1F
			Instruction("sla B", 2, { }), // 0x20
			Instruction("sla C", 2, { }), // 0x21
			Instruction("sla D", 2, { }), // 0x22
			Instruction("sla E", 2, { }), // 0x23
			Instruction("sla H", 2, { }), // 0x24
			Instruction("sla L", 2, { }), // 0x25
			Instruction("sla (HL)", 2, { }), // 0x26
			Instruction("sla A", 2, { }), // 0x27
			Instruction("sra B", 2, { }), // 0x28
			Instruction("sra C", 2, { }), // 0x29
			Instruction("sra D", 2, { }), // 0x2A
			Instruction("sra E", 2, { }), // 0x2B
			Instruction("sra H", 2, { }), // 0x2C
			Instruction("sra L", 2, { }), // 0x2D
			Instruction("sra (HL)", 2, { }), // 0x2E
			Instruction("sra A", 2, { }), // 0x2F
			Instruction("swap B", 2, { }), // 0x30
			Instruction("swap C", 2, { }), // 0x31
			Instruction("swap D", 2, { }), // 0x32
			Instruction("swap E", 2, { }), // 0x33
			Instruction("swap H", 2, { }), // 0x34
			Instruction("swap L", 2, { }), // 0x35
			Instruction("swap (HL)", 2, { }), // 0x36
			Instruction("swap A", 2, { }), // 0x37
			Instruction("srl B", 2, { }), // 0x38
			Instruction("srl C", 2, { }), // 0x39
			Instruction("srl D", 2, { }), // 0x3A
			Instruction("srl E", 2, { }), // 0x3B
			Instruction("srl H", 2, { }), // 0x3C
			Instruction("srl L", 2, { }), // 0x3D
			Instruction("srl (HL)", 2, { }), // 0x3E
			Instruction("srl A", 2, { }), // 0x3F
			Instruction("bit 0, B", 2, { }), // 0x40
			Instruction("bit 0, C", 2, { }), // 0x41
			Instruction("bit 0, D", 2, { }), // 0x42
			Instruction("bit 0, E", 2, { }), // 0x43
			Instruction("bit 0, H", 2, { }), // 0x44
			Instruction("bit 0, L", 2, { }), // 0x45
			Instruction("bit 0, (HL)", 2, { }), // 0x46
			Instruction("bit 0, A", 2, { }), // 0x47
			Instruction("bit 1, B", 2, { }), // 0x48
			Instruction("bit 1, C", 2, { }), // 0x49
			Instruction("bit 1, D", 2, { }), // 0x4A
			Instruction("bit 1, E", 2, { }), // 0x4B
			Instruction("bit 1, H", 2, { }), // 0x4C
			Instruction("bit 1, L", 2, { }), // 0x4D
			Instruction("bit 1, (HL)", 2, { }), // 0x4E
			Instruction("bit 1, A", 2, { }), // 0x4F
			Instruction("bit 2, B", 2, { }), // 0x50
			Instruction("bit 2, C", 2, { }), // 0x51
			Instruction("bit 2, D", 2, { }), // 0x52
			Instruction("bit 2, E", 2, { }), // 0x53
			Instruction("bit 2, H", 2, { }), // 0x54
			Instruction("bit 2, L", 2, { }), // 0x55
			Instruction("bit 2, (HL)", 2, { }), // 0x56
			Instruction("bit 2, A", 2, { }), // 0x57
			Instruction("bit 3, B", 2, { }), // 0x58
			Instruction("bit 3, C", 2, { }), // 0x59
			Instruction("bit 3, D", 2, { }), // 0x5A
			Instruction("bit 3, E", 2, { }), // 0x5B
			Instruction("bit 3, H", 2, { }), // 0x5C
			Instruction("bit 3, L", 2, { }), // 0x5D
			Instruction("bit 3, (HL)", 2, { }), // 0x5E
			Instruction("bit 3, A", 2, { }), // 0x5F
			Instruction("bit 4, B", 2, { }), // 0x60
			Instruction("bit 4, C", 2, { }), // 0x61
			Instruction("bit 4, D", 2, { }), // 0x62
			Instruction("bit 4, E", 2, { }), // 0x63
			Instruction("bit 4, H", 2, { }), // 0x64
			Instruction("bit 4, L", 2, { }), // 0x65
			Instruction("bit 4, (HL)", 2, { }), // 0x66
			Instruction("bit 4, A", 2, { }), // 0x67
			Instruction("bit 5, B", 2, { }), // 0x68
			Instruction("bit 5, C", 2, { }), // 0x69
			Instruction("bit 5, D", 2, { }), // 0x6A
			Instruction("bit 5, E", 2, { }), // 0x6B
			Instruction("bit 5, H", 2, { }), // 0x6C
			Instruction("bit 5, L", 2, { }), // 0x6D
			Instruction("bit 5, (HL)", 2, { }), // 0x6E
			Instruction("bit 5, A", 2, { }), // 0x6F
			Instruction("bit 6, B", 2, { }), // 0x70
			Instruction("bit 6, C", 2, { }), // 0x71
			Instruction("bit 6, D", 2, { }), // 0x72
			Instruction("bit 6, E", 2, { }), // 0x73
			Instruction("bit 6, H", 2, { }), // 0x74
			Instruction("bit 6, L", 2, { }), // 0x75
			Instruction("bit 6, (HL)", 2, { }), // 0x76
			Instruction("bit 6, A", 2, { }), // 0x77
			Instruction("bit 7, B", 2, { }), // 0x78
			Instruction("bit 7, C", 2, { }), // 0x79
			Instruction("bit 7, D", 2, { }), // 0x7A
			Instruction("bit 7, E", 2, { }), // 0x7B
			Instruction("bit 7, H", 2, { }), // 0x7C
			Instruction("bit 7, L", 2, { }), // 0x7D
			Instruction("bit 7, (HL)", 2, { }), // 0x7E
			Instruction("bit 7, A", 2, { }), // 0x7F
			Instruction("res 0, B", 2, { }), // 0x80
			Instruction("res 0, C", 2, { }), // 0x81
			Instruction("res 0, D", 2, { }), // 0x82
			Instruction("res 0, E", 2, { }), // 0x83
			Instruction("res 0, H", 2, { }), // 0x84
			Instruction("res 0, L", 2, { }), // 0x85
			Instruction("res 0, (HL)", 2, { }), // 0x86
			Instruction("res 0, A", 2, { }), // 0x87
			Instruction("res 1, B", 2, { }), // 0x88
			Instruction("res 1, C", 2, { }), // 0x89
			Instruction("res 1, D", 2, { }), // 0x8A
			Instruction("res 1, E", 2, { }), // 0x8B
			Instruction("res 1, H", 2, { }), // 0x8C
			Instruction("res 1, L", 2, { }), // 0x8D
			Instruction("res 1, (HL)", 2, { }), // 0x8E
			Instruction("res 1, A", 2, { }), // 0x8F
			Instruction("res 2, B", 2, { }), // 0x90
			Instruction("res 2, C", 2, { }), // 0x91
			Instruction("res 2, D", 2, { }), // 0x92
			Instruction("res 2, E", 2, { }), // 0x93
			Instruction("res 2, H", 2, { }), // 0x94
			Instruction("res 2, L", 2, { }), // 0x95
			Instruction("res 2, (HL)", 2, { }), // 0x96
			Instruction("res 2, A", 2, { }), // 0x97
			Instruction("res 3, B", 2, { }), // 0x98
			Instruction("res 3, C", 2, { }), // 0x99
			Instruction("res 3, D", 2, { }), // 0x9A
			Instruction("res 3, E", 2, { }), // 0x9B
			Instruction("res 3, H", 2, { }), // 0x9C
			Instruction("res 3, L", 2, { }), // 0x9D
			Instruction("res 3, (HL)", 2, { }), // 0x9E
			Instruction("res 3, A", 2, { }), // 0x9F
			Instruction("res 4, B", 2, { }), // 0xA0
			Instruction("res 4, C", 2, { }), // 0xA1
			Instruction("res 4, D", 2, { }), // 0xA2
			Instruction("res 4, E", 2, { }), // 0xA3
			Instruction("res 4, H", 2, { }), // 0xA4
			Instruction("res 4, L", 2, { }), // 0xA5
			Instruction("res 4, (HL)", 2, { }), // 0xA6
			Instruction("res 4, A", 2, { }), // 0xA7
			Instruction("res 5, B", 2, { }), // 0xA8
			Instruction("res 5, C", 2, { }), // 0xA9
			Instruction("res 5, D", 2, { }), // 0xAA
			Instruction("res 5, E", 2, { }), // 0xAB
			Instruction("res 5, H", 2, { }), // 0xAC
			Instruction("res 5, L", 2, { }), // 0xAD
			Instruction("res 5, (HL)", 2, { }), // 0xAE
			Instruction("res 5, A", 2, { }), // 0xAF
			Instruction("res 6, B", 2, { }), // 0xB0
			Instruction("res 6, C", 2, { }), // 0xB1
			Instruction("res 6, D", 2, { }), // 0xB2
			Instruction("res 6, E", 2, { }), // 0xB3
			Instruction("res 6, H", 2, { }), // 0xB4
			Instruction("res 6, L", 2, { }), // 0xB5
			Instruction("res 6, (HL)", 2, { }), // 0xB6
			Instruction("res 6, A", 2, { }), // 0xB7
			Instruction("res 7, B", 2, { }), // 0xB8
			Instruction("res 7, C", 2, { }), // 0xB9
			Instruction("res 7, D", 2, { }), // 0xBA
			Instruction("res 7, E", 2, { }), // 0xBB
			Instruction("res 7, H", 2, { }), // 0xBC
			Instruction("res 7, L", 2, { }), // 0xBD
			Instruction("res 7, (HL)", 2, { }), // 0xBE
			Instruction("res 7, A", 2, { }), // 0xBF
			Instruction("set 0, B", 2, { }), // 0xC0
			Instruction("set 0, C", 2, { }), // 0xC1
			Instruction("set 0, D", 2, { }), // 0xC2
			Instruction("set 0, E", 2, { }), // 0xC3
			Instruction("set 0, H", 2, { }), // 0xC4
			Instruction("set 0, L", 2, { }), // 0xC5
			Instruction("set 0, (HL)", 2, { }), // 0xC6
			Instruction("set 0, A", 2, { }), // 0xC7
			Instruction("set 1, B", 2, { }), // 0xC8
			Instruction("set 1, C", 2, { }), // 0xC9
			Instruction("set 1, D", 2, { }), // 0xCA
			Instruction("set 1, E", 2, { }), // 0xCB
			Instruction("set 1, H", 2, { }), // 0xCC
			Instruction("set 1, L", 2, { }), // 0xCD
			Instruction("set 1, (HL)", 2, { }), // 0xCE
			Instruction("set 1, A", 2, { }), // 0xCF
			Instruction("set 2, B", 2, { }), // 0xD0
			Instruction("set 2, C", 2, { }), // 0xD1
			Instruction("set 2, D", 2, { }), // 0xD2
			Instruction("set 2, E", 2, { }), // 0xD3
			Instruction("set 2, H", 2, { }), // 0xD4
			Instruction("set 2, L", 2, { }), // 0xD5
			Instruction("set 2, (HL)", 2, { }), // 0xD6
			Instruction("set 2, A", 2, { }), // 0xD7
			Instruction("set 3, B", 2, { }), // 0xD8
			Instruction("set 3, C", 2, { }), // 0xD9
			Instruction("set 3, D", 2, { }), // 0xDA
			Instruction("set 3, E", 2, { }), // 0xDB
			Instruction("set 3, H", 2, { }), // 0xDC
			Instruction("set 3, L", 2, { }), // 0xDD
			Instruction("set 3, (HL)", 2, { }), // 0xDE
			Instruction("set 3, A", 2, { }), // 0xDF
			Instruction("set 4, B", 2, { }), // 0xE0
			Instruction("set 4, C", 2, { }), // 0xE1
			Instruction("set 4, D", 2, { }), // 0xE2
			Instruction("set 4, E", 2, { }), // 0xE3
			Instruction("set 4, H", 2, { }), // 0xE4
			Instruction("set 4, L", 2, { }), // 0xE5
			Instruction("set 4, (HL)", 2, { }), // 0xE6
			Instruction("set 4, A", 2, { }), // 0xE7
			Instruction("set 5, B", 2, { }), // 0xE8
			Instruction("set 5, C", 2, { }), // 0xE9
			Instruction("set 5, D", 2, { }), // 0xEA
			Instruction("set 5, E", 2, { }), // 0xEB
			Instruction("set 5, H", 2, { }), // 0xEC
			Instruction("set 5, L", 2, { }), // 0xED
			Instruction("set 5, (HL)", 2, { }), // 0xEE
			Instruction("set 5, A", 2, { }), // 0xEF
			Instruction("set 6, B", 2, { }), // 0xF0
			Instruction("set 6, C", 2, { }), // 0xF1
			Instruction("set 6, D", 2, { }), // 0xF2
			Instruction("set 6, E", 2, { }), // 0xF3
			Instruction("set 6, H", 2, { }), // 0xF4
			Instruction("set 6, L", 2, { }), // 0xF5
			Instruction("set 6, (HL)", 2, { }), // 0xF6
			Instruction("set 6, A", 2, { }), // 0xF7
			Instruction("set 7, B", 2, { }), // 0xF8
			Instruction("set 7, C", 2, { }), // 0xF9
			Instruction("set 7, D", 2, { }), // 0xFA
			Instruction("set 7, E", 2, { }), // 0xFB
			Instruction("set 7, H", 2, { }), // 0xFC
			Instruction("set 7, L", 2, { }), // 0xFD
			Instruction("set 7, (HL)", 2, { }), // 0xFE
			Instruction("set 7, A", 2, { }), // 0xFF
		};
	public:
		const Instruction* GetInstruction(const std::vector<u64>& opcodes) override;
		std::string Disassemble(const DecodeInfo& info) override;
		DecodeInfo Decode(Common::InstructionStream& stream) override;
	};

	class Core : public Common::Core
	{
	public:
		Core(Common::CoreLoader& loader);
		
		const std::string GetName() const override;
		const std::string GetDescription() const override;
		Common::Core::Type GetType() const override;
		std::unordered_map<std::string, Common::PropertyValue> GetDefaultProperties() const override;

		std::shared_ptr<Common::CoreInstance> CreateNewInstance(
			Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& propertyOverrides = {}) override;		
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
		const std::shared_ptr<Common::Logger> logger = runState.logManager.CreateLogger(core->GetName());

		Instance(Common::Core* core, Common::RunState& runState, const std::unordered_map<std::string, Common::PropertyValue>& propertyOverrides);

		ReturnStatus Init() override;
		ReturnStatus Step() override;

		std::string Disassemble(const std::vector<u8>& data) override;
		const Common::CoreState& GetCoreState() const override;
		std::chrono::nanoseconds GetStepPeriod() const override;
	};
}
