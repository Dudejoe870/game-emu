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
			std::string format;
			std::variant<u32*, u64*, u8*, u16*, float*, double*, bool*> pValue;
			std::endian endian;

			DebugRegisterInfo(const std::string& name, const std::string& format, std::variant<u32*, u64*, u8*, u16*, float*, double*, bool*> pValue, std::endian endian)
			{
				this->name = name;
				this->format = format;
				this->pValue = pValue;
				this->endian = endian;
			}

			inline bool isFloatingPoint()
			{
				return std::holds_alternative<float*>(pValue) || std::holds_alternative<double*>(pValue);
			}

			inline std::string getFormatted()
			{
				return fmt::format(fmt::runtime(format), 
					fmt::arg("name", name),
					fmt::arg("u8", getValue<u8>()),
					fmt::arg("u16", getValue<u16>()),
					fmt::arg("u32", getValue<u32>()),
					fmt::arg("u64", getValue<u64>()),
					fmt::arg("f32", getValue<float>()),
					fmt::arg("f64", getValue<double>()),
					fmt::arg("bool", getValue<bool>()));
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
