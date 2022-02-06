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
		inline float ToNativeEndian(float value)
		{
			static_assert(sizeof(float) == sizeof(unsigned int));
			unsigned int iValue = *reinterpret_cast<unsigned int*>(&value);
			iValue = ToNativeEndian<endian>(iValue);
			return *reinterpret_cast<float*>(&iValue);
		}

		template <std::endian endian>
		inline double ToNativeEndian(double value)
		{
			static_assert(sizeof(double) == sizeof(unsigned long long));
			unsigned long long iValue = *reinterpret_cast<unsigned long long*>(&value);
			iValue = ToNativeEndian<endian>(iValue);
			return *reinterpret_cast<double*>(&iValue);
		}

		template <std::endian endian>
		inline unsigned char ToNativeEndian(unsigned char value)
		{
			return value;
		}

		template <std::endian endian>
		inline bool ToNativeEndian(bool value)
		{
			return value;
		}

		inline unsigned long long ToNativeEndian(unsigned long long value, std::endian endian)
		{
			if (std::endian::native != endian)
				return bswap_64(value);
			else
				return value;
		}

		inline unsigned int ToNativeEndian(unsigned int value, std::endian endian)
		{
			if (std::endian::native != endian)
				return bswap_32(value);
			else
				return value;
		}

		inline unsigned short ToNativeEndian(unsigned short value, std::endian endian)
		{
			if (std::endian::native != endian)
				return bswap_16(value);
			else
				return value;
		}

		inline float ToNativeEndian(float value, std::endian endian)
		{
			static_assert(sizeof(float) == sizeof(unsigned int));
			unsigned int iValue = *reinterpret_cast<unsigned int*>(&value);
			iValue = ToNativeEndian(iValue, endian);
			return *reinterpret_cast<float*>(&iValue);
		}

		inline double ToNativeEndian(double value, std::endian endian)
		{
			static_assert(sizeof(double) == sizeof(unsigned long long));
			unsigned long long iValue = *reinterpret_cast<unsigned long long*>(&value);
			iValue = ToNativeEndian(iValue, endian);
			return *reinterpret_cast<double*>(&iValue);
		}

		inline unsigned char ToNativeEndian(unsigned char value, std::endian endian)
		{
			return value;
		}

		inline bool ToNativeEndian(bool value, std::endian endian)
		{
			return value;
		}
	}
}
