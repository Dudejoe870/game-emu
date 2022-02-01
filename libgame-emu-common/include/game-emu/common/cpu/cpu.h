#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/cpu/instructiondecoder.h>

namespace GameEmu::Common::Cpu
{
	class CPU
	{
	public:
		enum class ReturnStatus
		{
			Success,
			Error
		};

		/*
		 Steps the CPU logic.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual ReturnStatus Step();

		/*
		 Steps the CPU logic.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT virtual InstructionDecoder* getInstructionDecoder();
	};
}
