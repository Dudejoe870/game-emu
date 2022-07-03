#pragma once

#include <game-emu/common/stdcommon.h>

#ifdef __linux__
#include <byteswap.h>
#else
#define bswap_16(x) _byteswap_ushort(x)
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)
#endif

namespace GameEmu::Common
{
	namespace Util
	{
		template <std::endian endian>
		inline u64 ToNativeEndian(u64 value)
		{
			if constexpr (std::endian::native != endian)
				return bswap_64(value);
			else
				return value;
		}

		template <std::endian endian>
		inline u32 ToNativeEndian(u32 value)
		{
			if constexpr (std::endian::native != endian)
				return bswap_32(value);
			else
				return value;
		}

		template <std::endian endian>
		inline u16 ToNativeEndian(u16 value)
		{
			if constexpr (std::endian::native != endian)
				return bswap_16(value);
			else
				return value;
		}

		template <std::endian endian>
		inline float ToNativeEndian(float value)
		{
			static_assert(sizeof(float) == sizeof(u32));
			u32 iValue = *reinterpret_cast<u32*>(&value);
			iValue = ToNativeEndian<endian>(iValue);
			return *reinterpret_cast<float*>(&iValue);
		}

		template <std::endian endian>
		inline double ToNativeEndian(double value)
		{
			static_assert(sizeof(double) == sizeof(u64));
			u64 iValue = *reinterpret_cast<u64*>(&value);
			iValue = ToNativeEndian<endian>(iValue);
			return *reinterpret_cast<double*>(&iValue);
		}

		template <std::endian endian>
		inline u8 ToNativeEndian(u8 value)
		{
			return value;
		}

		template <std::endian endian>
		inline bool ToNativeEndian(bool value)
		{
			return value;
		}

		inline u64 ToNativeEndian(u64 value, std::endian endian)
		{
			if (std::endian::native != endian)
				return bswap_64(value);
			else
				return value;
		}

		inline u32 ToNativeEndian(u32 value, std::endian endian)
		{
			if (std::endian::native != endian)
				return bswap_32(value);
			else
				return value;
		}

		inline u16 ToNativeEndian(u16 value, std::endian endian)
		{
			if (std::endian::native != endian)
				return bswap_16(value);
			else
				return value;
		}

		inline float ToNativeEndian(float value, std::endian endian)
		{
			static_assert(sizeof(float) == sizeof(u32));
			u32 iValue = *reinterpret_cast<u32*>(&value);
			iValue = ToNativeEndian(iValue, endian);
			return *reinterpret_cast<float*>(&iValue);
		}

		inline double ToNativeEndian(double value, std::endian endian)
		{
			static_assert(sizeof(double) == sizeof(u64));
			u64 iValue = *reinterpret_cast<u64*>(&value);
			iValue = ToNativeEndian(iValue, endian);
			return *reinterpret_cast<double*>(&iValue);
		}

		inline u8 ToNativeEndian(u8 value, std::endian endian)
		{
			return value;
		}

		inline bool ToNativeEndian(bool value, std::endian endian)
		{
			return value;
		}
	}
}
