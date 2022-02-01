#pragma once

#include <game-emu/common/stdcommon.h>

namespace GameEmu::Common::Cpu
{
	class InstructionStream
	{
	public:
		/*
		 Get next Instruction section (could be a byte stream like x86, could be a single full 32-bit instruction like mips)
		*/
		virtual bool getNext(unsigned long long& value) = 0;
	};

	template <class T, std::endian endian>
	class VectorInstructionStream : public InstructionStream
	{
	private:
		const std::vector<unsigned char>& data;
		unsigned int offset;
	public:
		VectorInstructionStream(const std::vector<unsigned char>& data, unsigned int offset)
			: data(data)
		{
			this->offset = offset;
		}

		bool getNext(unsigned long long& value)
		{
			if (offset + (sizeof(T)-1) >= data.size()) return false;
			unsigned long long result = static_cast<unsigned long long>(
				Util::ToNativeEndian<endian>(*reinterpret_cast<const T*>(&data[offset])));
			offset += sizeof(T);
			value = result;
			return true;
		}
	};
}
