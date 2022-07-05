#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/symbolhelper.h>

#include <game-emu/common/endianutils.h>

namespace GameEmu::Common
{
	/*
	 Represents a Physical Address space that can have entries mapped in it in some way.
	 Internal structure could be anything from a binary search tree, to a linear lookup table.

	 Implementations of this class must satisfy the requirement of
	 calling the readEvent and writeEvent functions on reads and writes to those memory regions.
	*/
	class PhysicalMemoryMap
	{
	private:
		// We have to export these too because they are used in inline functions.
		LIBGAMEEMU_COMMON_DLL_EXPORT void WriteU16BigEndianImpl(u16 value, u64 address);
		LIBGAMEEMU_COMMON_DLL_EXPORT void WriteU16LittleEndianImpl(u16 value, u64 address);

		LIBGAMEEMU_COMMON_DLL_EXPORT u16 ReadU16BigEndianImpl(u64 address);
		LIBGAMEEMU_COMMON_DLL_EXPORT u16 ReadU16LittleEndianImpl(u64 address);

		LIBGAMEEMU_COMMON_DLL_EXPORT void WriteU32BigEndianImpl(u32 value, u64 address);
		LIBGAMEEMU_COMMON_DLL_EXPORT void WriteU32LittleEndianImpl(u32 value, u64 address);

		LIBGAMEEMU_COMMON_DLL_EXPORT u32 ReadU32BigEndianImpl(u64 address);
		LIBGAMEEMU_COMMON_DLL_EXPORT u32 ReadU32LittleEndianImpl(u64 address);

		LIBGAMEEMU_COMMON_DLL_EXPORT void WriteU64BigEndianImpl(u64 value, u64 address);
		LIBGAMEEMU_COMMON_DLL_EXPORT void WriteU64LittleEndianImpl(u64 value, u64 address);

		LIBGAMEEMU_COMMON_DLL_EXPORT u64 ReadU64BigEndianImpl(u64 address);
		LIBGAMEEMU_COMMON_DLL_EXPORT u64 ReadU64LittleEndianImpl(u64 address);

		u64 addressMask;
	public:
		/*
		 8-bit Write
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT void WriteU8(u8 value, u64 address);		
		inline void WriteS8(s8 value, u64 address)
		{
			WriteU8(static_cast<u8>(value), address);
		}

		/*
		 8-bit Read
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT u8 ReadU8(u64 address);
		inline s8 ReadS8(u64 address)
		{
			return static_cast<s8>(ReadU8(address));
		}

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

		struct Entry
		{
			u64 address;

			u8* hostReadMemory;
			u8* hostWriteMemory;
			u64 size;

			/*
			 Returns the value to be read.

			 readValue is the original value stored in the read memory.
			 writeValue is the original value stored in the write memory.
			 address is the address being read from.
			 size is the size of the read.
			*/
			using ReadEventFunction = std::function<u64(u64 readValue, u64 writeValue, u64 address, u8 size)>;

			/*
			 Returns the value to be written.

			 readValue is the original value stored in the read memory.
			 writeValue is the original value stored in the write memory.
			 valueToBeWritten is the original value being written.
			 address is the address being written to.
			 size is the size of the write.
			*/
			using WriteEventFunction = std::function<u64(u64 readValue, u64 writeValue, u64 valueToBeWritten, u64 address, u8 size)>;

			ReadEventFunction readEvent;
			WriteEventFunction writeEvent;

			Entry(u64 address, u8* hostReadMemory, u8* hostWriteMemory, u64 size, ReadEventFunction readEvent = nullptr, WriteEventFunction writeEvent = nullptr)
			{
				this->address = address;

				this->hostReadMemory = hostReadMemory;
				this->hostWriteMemory = hostWriteMemory;
				this->size = size;

				this->readEvent = readEvent;
				this->writeEvent = writeEvent;
			}
		};
		
		virtual const std::shared_ptr<Entry> FindEntry(u64 address) = 0;
	private:
		template <class T, std::endian endian>
		void WriteImpl(u64 value, u64 address)
		{
			address &= addressMask;

			std::shared_ptr<Entry> entry = FindEntry(address);
			if (!entry) return;

			u64 offset = address - entry->address;
			if ((offset + (sizeof(T) - 1)) >= entry->size) return;
			T* pWriteValue = reinterpret_cast<T*>(&entry->hostWriteMemory[offset]);

			if (entry->writeEvent)
			{
				T* pReadValue = reinterpret_cast<T*>(&entry->hostReadMemory[offset]);
				*pWriteValue = Util::ToNativeEndian<endian>(static_cast<T>(
					entry->writeEvent(
						static_cast<u64>(Util::ToNativeEndian<endian>(*pReadValue)),
						static_cast<u64>(Util::ToNativeEndian<endian>(*pWriteValue)),
						value, address, sizeof(T))));
			}
			else *pWriteValue = Util::ToNativeEndian<endian>(static_cast<T>(value));
		}

		template <class T, std::endian endian>
		u64 ReadImpl(u64 address)
		{
			address &= addressMask;

			std::shared_ptr<Entry> entry = FindEntry(address);
			if (!entry) return 0;

			u64 offset = address - entry->address;
			if ((offset + (sizeof(T) - 1)) >= entry->size) return 0;
			T* pReadValue = reinterpret_cast<T*>(&entry->hostReadMemory[offset]);

			if (entry->readEvent)
			{
				T* pWriteValue = reinterpret_cast<T*>(&entry->hostWriteMemory[offset]);
				return Util::ToNativeEndian<endian>(static_cast<T>(
					entry->readEvent(
						static_cast<u64>(Util::ToNativeEndian<endian>(*pReadValue)),
						static_cast<u64>(Util::ToNativeEndian<endian>(*pWriteValue)),
						address, sizeof(T))));
			}
			return Util::ToNativeEndian<endian>(*pReadValue);
		}
	public:
		PhysicalMemoryMap(u64 addressMask) 
		{
			this->addressMask = addressMask;
		}

		virtual const std::shared_ptr<Entry> Map(u8* hostReadMemory, u8* hostWriteMemory, u64 size, u64 baseAddress,
			Entry::ReadEventFunction readEvent = nullptr, Entry::WriteEventFunction writeEvent = nullptr) = 0;
		inline const std::shared_ptr<Entry> Map(u8* hostMemory, u64 size, u64 baseAddress,
			Entry::ReadEventFunction readEvent = nullptr, Entry::WriteEventFunction writeEvent = nullptr)
		{
			return Map(hostMemory, hostMemory, size, baseAddress, readEvent, writeEvent);
		}

		virtual void Unmap(const std::shared_ptr<Entry>& entry) = 0;
	};

	/*
	 A Physical Memory Map implementation
	 with an internal Binary Search Tree allowing for better performance than linear searches.

	 Should be the default go-to for Memory Mapping unless the specific system benefits from a separate approach.
	*/
	class BinaryTreeMemoryMap : public PhysicalMemoryMap
	{
	private:
		struct Node
		{
			std::shared_ptr<Entry> entry;

			std::shared_ptr<Node> parent;
			std::shared_ptr<Node> left;
			std::shared_ptr<Node> right;

			Node(const std::shared_ptr<Entry>& entry, const std::shared_ptr<Node>& parent, 
				const std::shared_ptr<Node>& left = nullptr, const std::shared_ptr<Node>& right = nullptr)
			{
				this->entry = entry;

				this->parent = parent;
				this->left = left;
				this->right = right;
			}
		};

		std::shared_ptr<Node> root = nullptr;
	public:
		BinaryTreeMemoryMap(u64 addressMask)
			: PhysicalMemoryMap(addressMask)
		{
		}

		LIBGAMEEMU_COMMON_DLL_EXPORT const std::shared_ptr<Entry> FindEntry(u64 address) override;
		LIBGAMEEMU_COMMON_DLL_EXPORT const std::shared_ptr<Entry> Map(u8* hostReadMemory, u8* hostWriteMemory, u64 size, u64 baseAddress,
			Entry::ReadEventFunction readEvent = nullptr, Entry::WriteEventFunction writeEvent = nullptr) override;
		LIBGAMEEMU_COMMON_DLL_EXPORT void Unmap(const std::shared_ptr<Entry>& entry) override;
	};
}
