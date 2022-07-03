#pragma once

#include <game-emu/common/stdcommon.h>

namespace GameEmu::Common
{
	/*
	 Represents a Physical Address space.

	 Implementations of this class must satisfy the requirement of 
	 treating the addresses passed into functions as "address & addressMask",
	 as well as correct endian swapping depending on the function.
	*/
	class PhysicalAddressTranslator
	{
	protected:
		virtual void WriteU16BigEndianImpl(u16 value, u64 address) = 0;
		virtual void WriteU16LittleEndianImpl(u16 value, u64 address) = 0;

		virtual u16 ReadU16BigEndianImpl(u64 address) = 0;
		virtual u16 ReadU16LittleEndianImpl(u64 address) = 0;

		virtual void WriteU32BigEndianImpl(u32 value, u64 address) = 0;
		virtual void WriteU32LittleEndianImpl(u32 value, u64 address) = 0;

		virtual u32 ReadU32BigEndianImpl(u64 address) = 0;
		virtual u32 ReadU32LittleEndianImpl(u64 address) = 0;

		virtual void WriteU64BigEndianImpl(u64 value, u64 address) = 0;
		virtual void WriteU64LittleEndianImpl(u64 value, u64 address) = 0;

		virtual u64 ReadU64BigEndianImpl(u64 address) = 0;
		virtual u64 ReadU64LittleEndianImpl(u64 address) = 0;

		u64 addressMask;
	public:
		PhysicalAddressTranslator(u64 addressMask)
		{
			this->addressMask = addressMask;
		}

		virtual ~PhysicalAddressTranslator() = default;

		/*
		 8-bit Write
		*/
		virtual void WriteU8(u8 value, u64 address) = 0;
		inline void WriteS8(s8 value, u64 address)
		{
			WriteU8(static_cast<u8>(value), address);
		}

		/*
		 8-bit Read
		*/
		virtual u8 ReadU8(u64 address) = 0;
		inline s8 ReadS8(u64 address)
		{
			return static_cast<s8>(ReadU8(address));
		}

		// Yay templates! ._. (this was way more lines of code than I thought it'd be) - JHC

		/*
		 16-bit Write
		*/
		template <std::endian endian>
		void WriteU16(u16 value, u64 address)
		{
			if constexpr (endian == std::endian::big)
				WriteU16BigEndianImpl(value, address);
			else if constexpr (endian == std::endian::little)
				WriteU16LittleEndianImpl(value, address);
		}

		inline void WriteU16(u16 value, u64 address, std::endian endian)
		{
			if (endian == std::endian::big) 
				WriteU16BigEndianImpl(value, address);
			else if (endian == std::endian::little) 
				WriteU16LittleEndianImpl(value, address);
		}

		template <std::endian endian>
		void WriteS16(s16 value, u64 address)
		{
			if constexpr (endian == std::endian::big)
				WriteU16BigEndianImpl(static_cast<u16>(value), address);
			else if constexpr (endian == std::endian::little)
				WriteU16LittleEndianImpl(static_cast<u16>(value), address);
		}

		inline void WriteS16(s16 value, u64 address, std::endian endian)
		{
			if (endian == std::endian::big) 
				WriteU16BigEndianImpl(static_cast<u16>(value), address);
			else if (endian == std::endian::little) 
				WriteU16LittleEndianImpl(static_cast<u16>(value), address);
		}

		/*
		 16-bit Read
		*/
		template <std::endian endian>
		u16 ReadU16(u64 address)
		{
			if constexpr (endian == std::endian::big)
				return ReadU16BigEndianImpl(address);
			return ReadU16LittleEndianImpl(address);
		}

		u16 ReadU16(u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				return ReadU16BigEndianImpl(address);
			return ReadU16LittleEndianImpl(address);
		}

		template <std::endian endian>
		s16 ReadS16(u64 address)
		{
			if constexpr (endian == std::endian::big)
				return static_cast<s16>(ReadU16BigEndianImpl(address));
			return static_cast<s16>(ReadU16LittleEndianImpl(address));
		}

		s16 ReadS16(u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				return static_cast<s16>(ReadU16BigEndianImpl(address));
			return static_cast<s16>(ReadU16LittleEndianImpl(address));
		}

		/*
		 32-bit Write
		*/
		template <std::endian endian>
		void WriteU32(u32 value, u64 address)
		{
			if constexpr (endian == std::endian::big)
				WriteU32BigEndianImpl(value, address);
			else if constexpr (endian == std::endian::little)
				WriteU32LittleEndianImpl(value, address);
		}

		inline void WriteU32(u32 value, u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				WriteU32BigEndianImpl(value, address);
			else if (endian == std::endian::little)
				WriteU32LittleEndianImpl(value, address);
		}

		template <std::endian endian>
		void WriteS32(s32 value, u64 address)
		{
			if constexpr (endian == std::endian::big)
				WriteU32BigEndianImpl(static_cast<u32>(value), address);
			else if constexpr (endian == std::endian::little)
				WriteU32LittleEndianImpl(static_cast<u32>(value), address);
		}

		inline void WriteS32(s32 value, u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				WriteU32BigEndianImpl(static_cast<u32>(value), address);
			else if (endian == std::endian::little)
				WriteU32LittleEndianImpl(static_cast<u32>(value), address);
		}

		/*
		 32-bit Read
		*/
		template <std::endian endian>
		u32 ReadU32(u64 address)
		{
			if constexpr (endian == std::endian::big)
				return ReadU32BigEndianImpl(address);
			return ReadU32LittleEndianImpl(address);
		}

		u32 ReadU32(u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				return ReadU32BigEndianImpl(address);
			return ReadU32LittleEndianImpl(address);
		}

		template <std::endian endian>
		s32 ReadS32(u64 address)
		{
			if constexpr (endian == std::endian::big)
				return static_cast<s32>(ReadU32BigEndianImpl(address));
			return static_cast<s32>(ReadU32LittleEndianImpl(address));
		}

		s32 ReadS32(u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				return static_cast<s32>(ReadU32BigEndianImpl(address));
			return static_cast<s32>(ReadU32LittleEndianImpl(address));
		}

		/*
		 64-bit Write
		*/
		template <std::endian endian>
		void WriteU64(u64 value, u64 address)
		{
			if constexpr (endian == std::endian::big)
				WriteU64BigEndianImpl(value, address);
			else if constexpr (endian == std::endian::little)
				WriteU64LittleEndianImpl(value, address);
		}

		inline void WriteU64(u64 value, u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				WriteU64BigEndianImpl(value, address);
			else if (endian == std::endian::little)
				WriteU64LittleEndianImpl(value, address);
		}

		template <std::endian endian>
		void WriteS64(s64 value, u64 address)
		{
			if constexpr (endian == std::endian::big)
				WriteU64BigEndianImpl(static_cast<u64>(value), address);
			else if constexpr (endian == std::endian::little)
				WriteU64LittleEndianImpl(static_cast<u64>(value), address);
		}

		inline void WriteS64(s64 value, u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				WriteU64BigEndianImpl(static_cast<u64>(value), address);
			else if (endian == std::endian::little)
				WriteU64LittleEndianImpl(static_cast<u64>(value), address);
		}

		/*
		 64-bit Read
		*/
		template <std::endian endian>
		u64 ReadU64(u64 address)
		{
			if constexpr (endian == std::endian::big)
				return ReadU64BigEndianImpl(address);
			return ReadU64LittleEndianImpl(address);
		}

		u64 ReadU64(u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				return ReadU64BigEndianImpl(address);
			return ReadU64LittleEndianImpl(address);
		}

		template <std::endian endian>
		s64 ReadS64(u64 address)
		{
			if constexpr (endian == std::endian::big)
				return static_cast<s64>(ReadU64BigEndianImpl(address));
			return static_cast<s64>(ReadU64LittleEndianImpl(address));
		}

		s64 ReadS64(u64 address, std::endian endian)
		{
			if (endian == std::endian::big)
				return static_cast<s64>(ReadU64BigEndianImpl(address));
			return static_cast<s64>(ReadU64LittleEndianImpl(address));
		}

		/*
		 Generic Templates (templated endianess checking)
		*/
		template <class T, std::endian endian>
		void Write(T value, u64 address)
		{
			if constexpr (std::is_same_v<T, u8>)
				WriteU8(value, address);
			else if constexpr (std::is_same_v<T, s8>)
				WriteS8(value, address);
			else if constexpr (std::is_same_v<T, u16>)
				WriteU16<endian>(value, address);
			else if constexpr (std::is_same_v<T, s16>)
				WriteS16<endian>(value, address);
			else if constexpr (std::is_same_v<T, u32>)
				WriteU32<endian>(value, address);
			else if constexpr (std::is_same_v<T, s32>)
				WriteS32<endian>(value, address);
			else if constexpr (std::is_same_v<T, u64>)
				WriteU64<endian>(value, address);
			else if constexpr (std::is_same_v<T, s64>)
				WriteS64<endian>(value, address);
		}

		template <class T, std::endian endian>
		T Read(u64 address)
		{
			if constexpr (std::is_same_v<T, s8>)
				return ReadS8(address);
			else if constexpr (std::is_same_v<T, u16>)
				return ReadU16<endian>(address);
			else if constexpr (std::is_same_v<T, s16>)
				return ReadS16<endian>(address);
			else if constexpr (std::is_same_v<T, u32>)
				return ReadU32<endian>(address);
			else if constexpr (std::is_same_v<T, s32>)
				return ReadS32<endian>(address);
			else if constexpr (std::is_same_v<T, u64>)
				return ReadU64<endian>(address);
			else if constexpr (std::is_same_v<T, s64>)
				return ReadS64<endian>(address);
			return ReadU8(address);
		}

		/*
		 Generic Templates (runtime endianess checking)
		*/
		template <class T>
		void Write(T value, u64 address, std::endian endian)
		{
			if constexpr (std::is_same_v<T, u8>)
				WriteU8(value, address);
			else if constexpr (std::is_same_v<T, s8>)
				WriteS8(value, address);
			else if constexpr (std::is_same_v<T, u16>)
				WriteU16(value, address, endian);
			else if constexpr (std::is_same_v<T, s16>)
				WriteS16(value, address, endian);
			else if constexpr (std::is_same_v<T, u32>)
				WriteU32(value, address, endian);
			else if constexpr (std::is_same_v<T, s32>)
				WriteS32(value, address, endian);
			else if constexpr (std::is_same_v<T, u64>)
				WriteU64(value, address, endian);
			else if constexpr (std::is_same_v<T, s64>)
				WriteS64(value, address, endian);
		}

		template <class T>
		T Read(u64 address, std::endian endian)
		{
			if constexpr (std::is_same_v<T, s8>)
				return ReadS8(address);
			else if constexpr (std::is_same_v<T, u16>)
				return ReadU16(address, endian);
			else if constexpr (std::is_same_v<T, s16>)
				return ReadS16(address, endian);
			else if constexpr (std::is_same_v<T, u32>)
				return ReadU32(address, endian);
			else if constexpr (std::is_same_v<T, s32>)
				return ReadS32(address, endian);
			else if constexpr (std::is_same_v<T, u64>)
				return ReadU64(address, endian);
			else if constexpr (std::is_same_v<T, s64>)
				return ReadS64(address, endian);
			return ReadU8(address);
		}
	};
}
