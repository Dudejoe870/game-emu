#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/endianutils.h>

namespace GameEmu::Common
{
	class InstructionStream
	{
	public:
		/*
		 Get next Instruction section (could be a byte stream like x86, could be a single full 32-bit instruction like mips)
		*/
		virtual bool getNext(u64& value) = 0;
	};

	template <class T, std::endian endian>
	class VectorInstructionStream : public InstructionStream
	{
	private:
		const std::vector<u8>& data;
		u32 offset;
	public:
		VectorInstructionStream(const std::vector<u8>& data, u32 offset)
			: data(data)
		{
			this->offset = offset;
		}

		bool getNext(u64& value)
		{
			if (offset + (sizeof(T)-1) >= data.size()) return false;
			u64 result = static_cast<u64>(
				Util::ToNativeEndian<endian>(*reinterpret_cast<const T*>(&data[offset])));
			offset += sizeof(T);
			value = result;
			return true;
		}
	};
}
