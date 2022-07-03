#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/physicaladdresstranslator.h>
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
	class PhysicalMemoryMap : public PhysicalAddressTranslator
	{
	protected:
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
	public:
		LIBGAMEEMU_COMMON_DLL_EXPORT void WriteU8(u8 value, u64 address);
		LIBGAMEEMU_COMMON_DLL_EXPORT u8 ReadU8(u64 address);

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
			: PhysicalAddressTranslator(addressMask)
		{
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
	 with an internal Binary Tree allowing for better performance than linear searches.

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

		LIBGAMEEMU_COMMON_DLL_EXPORT const std::shared_ptr<Entry> FindEntry(u64 address);
		LIBGAMEEMU_COMMON_DLL_EXPORT const std::shared_ptr<Entry> Map(u8* hostReadMemory, u8* hostWriteMemory, u64 size, u64 baseAddress,
			Entry::ReadEventFunction readEvent = nullptr, Entry::WriteEventFunction writeEvent = nullptr);
		LIBGAMEEMU_COMMON_DLL_EXPORT void Unmap(const std::shared_ptr<Entry>& entry);
	};
}
