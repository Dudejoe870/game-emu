#pragma once

#include <game-emu/common/stdcommon.h>

#include <game-emu/common/corestate.h>

namespace GameEmu::Common
{
	/*
	 This class represents a "Register".
	 This could be a CPU register (in that case endian is recommended to be std::endian::native)
	 or a Memory Register, possibly mapped into a Cores memory space 
	 (in that case it should be whatever endian this register is on the native device, probably the CPUs native endian)
	*/
	template <class T, std::endian endian, bool events = true>
	class Register
	{
	private:
		T* pReadValue;
		T* pWriteValue;
	public:
		std::function<T(T value)> readEvent;
		std::function<T(T value, T writeValue)> writeEvent;

		Register(CoreState* coreState, T& readValue, T& writeValue, std::string debugName, std::function<T(T)> readEvent = nullptr, std::function<T(T, T)> writeEvent = nullptr)
		{
			this->pReadValue = &readValue;
			this->pWriteValue = &writeValue;
			this->readEvent = readEvent;
			this->writeEvent = writeEvent;
			if (coreState)
			{
				coreState->debugRegisters[debugName].pValue = pReadValue;
				coreState->debugRegisters[debugName].endian = endian;
			}
		}

		Register(CoreState* coreState, T& readValue, T& writeValue, std::function<T(T)> readEvent = nullptr, std::function<T(T, T)> writeEvent = nullptr)
		{
			this->pReadValue = &readValue;
			this->pWriteValue = &writeValue;
			this->readEvent = readEvent;
			this->writeEvent = writeEvent;
			if (coreState)
			{
				std::string debugName = "r" + coreState->debugRegisters.size();
				coreState->debugRegisters[debugName].pValue = pReadValue;
				coreState->debugRegisters[debugName].endian = endian;
			}
		}

		// Not recommended usage.
		Register(T& readValue, T& writeValue, std::function<T(T)> readEvent = nullptr, std::function<T(T, T)> writeEvent = nullptr)
		{
			this->pReadValue = &readValue;
			this->pWriteValue = &writeValue;
			this->readEvent = readEvent;
			this->writeEvent = writeEvent;
		}

		template<bool funcEvents = true>
		T Read()
		{
			if constexpr (funcEvents && events)
			{
				if (readEvent) return readEvent(Util::ToNativeEndian<endian>(*pReadValue));
			}
			return Util::ToNativeEndian<endian>(*pReadValue);
		}

		template<bool funcEvents = true>
		void Write(T value)
		{
			if constexpr (funcEvents && events)
			{
				if (writeEvent)
				{
					*pWriteValue = Util::ToNativeEndian<endian>(writeEvent(Util::ToNativeEndian<endian>(*pWriteValue), value));
					return;
				}
			}
			*pWriteValue = Util::ToNativeEndian<endian>(value);
		}

		template <class U, std::endian e, bool ev>
		Register& operator=(Register<U, e, ev>& reg)
		{
			Write<true>(static_cast<T>(reg.Read<true>()));
			return *this;
		}

		template <class U>
		Register& operator=(U value)
		{
			Write<true>(static_cast<T>(value));
			return *this;
		}

		template <class U>
		Register& operator+=(U value)
		{
			*this = *this + value;
			return *this;
		}

		template <class U>
		Register& operator-=(U value)
		{
			*this = *this - value;
			return *this;
		}

		template <class U>
		Register& operator/=(U value)
		{
			*this = *this / value;
			return *this;
		}

		template <class U>
		Register& operator*=(U value)
		{
			*this = *this * value;
			return *this;
		}

		template <class U>
		explicit operator U() { return static_cast<U>(Read<true>()); }

		operator T() { return Read<true>(); }

		template <class U, std::endian e, bool ev>
		friend class Register;
	};
}
