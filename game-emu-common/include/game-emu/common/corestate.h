#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/endianutils.h>

namespace GameEmu::Common
{
	class CoreState
	{
	public:
		struct DebugRegisterInfo
		{
			std::string name;
			std::variant<u32*, u64*, u8*, u16*, float*, double*, bool*> pValue;
			std::endian endian;

			DebugRegisterInfo(std::string name, std::variant<u32*, u64*, u8*, u16*, float*, double*, bool*> pValue, std::endian endian)
			{
				this->name = name;
				this->pValue = pValue;
				this->endian = endian;
			}

			inline bool isFloatingPoint()
			{
				return std::holds_alternative<float*>(pValue) || std::holds_alternative<double*>(pValue);
			}

			template <class T>
			T getValue()
			{
				// I am sorry for my sins. (this is debug code anyway so please don't make fun of me) - JHC
				if (std::holds_alternative<u32*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<u32*>(pValue), endian));
				else if (std::holds_alternative<u64*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<u64*>(pValue), endian));
				else if (std::holds_alternative<u8*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<u8*>(pValue), endian));
				else if (std::holds_alternative<u16*>(pValue))
					return static_cast<T>(Util::ToNativeEndian(*std::get<u16*>(pValue), endian));
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
		std::vector<DebugRegisterInfo> debugRegisters;
	public:
		inline std::vector<DebugRegisterInfo>& getRegisters()
		{
			return debugRegisters;
		}

		template <class, std::endian, bool> friend class Register;
	};
}
