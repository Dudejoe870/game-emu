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
	public:
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

			/*
			 For erasing.
			*/
			friend bool operator==(const Entry& l, const Entry& r)
			{
				return &l == &r;
			}

			/*
			 For sorting.
			*/
			friend bool operator<(const Entry& l, const Entry& r)
			{
				return l.address < r.address;
			}
		};

		PhysicalMemoryMap(u64 addressMask) 
			: PhysicalAddressTranslator(addressMask)
		{
		}

		virtual Entry* Map(u8* hostReadMemory, u8* hostWriteMemory, u64 size, u64 baseAddress,
			Entry::ReadEventFunction readEvent = nullptr, Entry::WriteEventFunction writeEvent = nullptr) = 0;
		inline Entry* Map(u8* hostMemory, u64 size, u64 baseAddress,
			Entry::ReadEventFunction readEvent = nullptr, Entry::WriteEventFunction writeEvent = nullptr)
		{
			return Map(hostMemory, hostMemory, size, baseAddress, readEvent, writeEvent);
		}

		virtual void Unmap(const Entry* entry) = 0;

		/*
		 Updates the Physical Address space with the current mappings.
		 (Could do nothing, depends on implementation)
		*/
		virtual void Update() = 0;
	};

	/*
	 A Physical Memory Map implementation
	 with an internal Binary Search Tree allowing for better performance than linear searches.

	 Should be the default go-to for Memory Mapping unless the specific system benefits from a separate approach.
	*/
	class BinaryTreeMemoryMap : public PhysicalMemoryMap
	{
	private:
		struct BinaryTree
		{
			struct Node
			{
				Entry data;

				std::shared_ptr<Node> left;
				std::shared_ptr<Node> right;

				Node(const Entry& data, std::shared_ptr<Node> left = nullptr, std::shared_ptr<Node> right = nullptr)
					: data(data)
				{
					this->left = left;
					this->right = right;
				}
			};

			std::shared_ptr<Node> root;
		};

		BinaryTree tree;

		std::vector<Entry> entries;

		std::shared_ptr<BinaryTree::Node> sortedVectorToBinaryTree(std::vector<Entry>& entries, s64 start, s64 end);

		inline std::shared_ptr<BinaryTree::Node> findNode(u64 address)
		{
			std::shared_ptr<BinaryTree::Node> currentNode = tree.root;

			while (currentNode)
			{
				u64 baseAddress = currentNode->data.address;
				u64 endAddress = currentNode->data.address + (currentNode->data.size - 1);

				if (address > baseAddress && address > endAddress)
				{
					currentNode = currentNode->right;
					continue;
				}
				else if (address < baseAddress)
				{
					currentNode = currentNode->left;
					continue;
				}
				else if (address >= baseAddress && address <= endAddress)
				{
					return currentNode;
				}
			}

			return nullptr;
		}

		template <class T, std::endian endian>
		void WriteImpl(u64 value, u64 address)
		{
			address &= addressMask;

			std::shared_ptr<BinaryTree::Node> node = findNode(address);
			if (!node) return;
			Entry& entry = node->data;

			u64 offset = address - entry.address;
			if (offset + (sizeof(T) - 1) >= entry.size) return;
			T* pWriteValue = reinterpret_cast<T*>(&entry.hostWriteMemory[offset]);

			if (entry.writeEvent)
			{
				T* pReadValue = reinterpret_cast<T*>(&entry.hostReadMemory[offset]);
				*pWriteValue = Util::ToNativeEndian<endian>(static_cast<T>(
					entry.writeEvent(
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

			std::shared_ptr<BinaryTree::Node> node = findNode(address);
			if (!node) return 0;
			Entry& entry = node->data;

			u64 offset = address - entry.address;
			if (offset + (sizeof(T) - 1) >= entry.size) return 0;
			T* pReadValue = reinterpret_cast<T*>(&entry.hostReadMemory[offset]);

			if (entry.readEvent)
			{
				T* pWriteValue = reinterpret_cast<T*>(&entry.hostWriteMemory[offset]);
				return Util::ToNativeEndian<endian>(static_cast<T>(
					entry.readEvent(
						static_cast<u64>(Util::ToNativeEndian<endian>(*pReadValue)),
						static_cast<u64>(Util::ToNativeEndian<endian>(*pWriteValue)),
						address, sizeof(T))));
			}
			return Util::ToNativeEndian<endian>(*pReadValue);
		}
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
		BinaryTreeMemoryMap(u64 addressMask)
			: PhysicalMemoryMap(addressMask)
		{
		}

		LIBGAMEEMU_COMMON_DLL_EXPORT void WriteU8(u8 value, u64 address);
		LIBGAMEEMU_COMMON_DLL_EXPORT u8 ReadU8(u64 address);

		LIBGAMEEMU_COMMON_DLL_EXPORT Entry* Map(u8* hostReadMemory, u8* hostWriteMemory, u64 size, u64 baseAddress,
			Entry::ReadEventFunction readEvent = nullptr, Entry::WriteEventFunction writeEvent = nullptr);
		LIBGAMEEMU_COMMON_DLL_EXPORT void Unmap(const Entry* entry);

		LIBGAMEEMU_COMMON_DLL_EXPORT void Update();
	};
}
