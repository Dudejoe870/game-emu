#pragma once

#include <game-emu/common/stdcommon.h>

namespace GameEmu::Common
{
	class CoreState
	{
	public:
		struct DebugRegisterInfo
		{
			std::variant<unsigned int*, unsigned long long*, unsigned char*, unsigned short*, float*, double*, bool*> pValue;
			std::endian endian;

			inline bool isFloatingPoint()
			{
				return std::holds_alternative<float*>(pValue) || std::holds_alternative<double*>(pValue);
			}

			template <class T>
			T getValue()
			{
				// I am sorry for my sins. (this is debug code anyway so please don't make fun of me) - JHC
				if (std::holds_alternative<unsigned int*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<unsigned int*>(pValue), endian));
				else if (std::holds_alternative<unsigned long long*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<unsigned long long*>(pValue), endian));
				else if (std::holds_alternative<unsigned char*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<unsigned char*>(pValue), endian));
				else if (std::holds_alternative<unsigned short*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<unsigned short*>(pValue), endian));
				else if (std::holds_alternative<float*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<float*>(pValue), endian));
				else if (std::holds_alternative<double*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<double*>(pValue), endian));
				else if (std::holds_alternative<bool*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<bool*>(pValue), endian));
				else throw std::runtime_error("invalid debug register type");
			}
		};
	private:
		std::unordered_map<std::string, DebugRegisterInfo> debugRegisters;
	public:
		inline std::unordered_map<std::string, DebugRegisterInfo>& getRegisters()
		{
			return debugRegisters;
		}

		template <class, std::endian, bool> friend class Register;
	};
}
