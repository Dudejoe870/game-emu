#pragma once

#include <bit>
#include <cstdlib>

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
		inline unsigned long long ToNativeEndian(unsigned long long value)
		{
			if constexpr (std::endian::native != endian)
				return bswap_64(value);
			else
				return value;
		}

		template <std::endian endian>
		inline unsigned int ToNativeEndian(unsigned int value)
		{
			if constexpr (std::endian::native != endian)
				return bswap_32(value);
			else
				return value;
		}

		template <std::endian endian>
		inline unsigned short ToNativeEndian(unsigned short value)
		{
			if constexpr (std::endian::native != endian)
				return bswap_16(value);
			else
				return value;
		}

		template <std::endian endian>
		inline unsigned char ToNativeEndian(unsigned char value)
		{
			return value;
		}
	}
}
