#include <game-emu/common/physicalmemorymap.h>

namespace GameEmu::Common
{
	PhysicalMemoryMap::Entry& BinaryTreeMemoryMap::Map(u8* hostReadMemory, u8* hostWriteMemory, u64 size, u64 baseAddress)
	{
		entries.push_back(Entry(baseAddress & addressMask, hostReadMemory, hostWriteMemory, size));
		return entries[entries.size() - 1];
	}

	void BinaryTreeMemoryMap::Unmap(const Entry& entry)
	{
		std::vector<Entry>::iterator entryIndex = std::find(entries.begin(), entries.end(), entry);
		if (entryIndex == entries.end()) return;

		entries.erase(entryIndex);
	}

	std::shared_ptr<BinaryTreeMemoryMap::BinaryTree::Node> BinaryTreeMemoryMap::sortedVectorToBinaryTree(
		std::vector<Entry>::iterator begin, std::vector<Entry>::iterator end)
	{
		if (begin > end)
			return nullptr;
		
		s64 mid = (end - begin) / 2;
		std::shared_ptr<BinaryTree::Node> root = std::make_shared<BinaryTree::Node>(begin[mid]);

		root->left = sortedVectorToBinaryTree(begin, begin + (mid - 1));

		root->right = sortedVectorToBinaryTree(begin + (mid + 1), end);

		return root;
	}

	void BinaryTreeMemoryMap::Update()
	{
		std::sort(entries.begin(), entries.end());

		// Rebalance the Binary Search Tree
		tree.root = sortedVectorToBinaryTree(entries.begin(), entries.end());
	}

	/*
	 8-bit Write
	*/

	void BinaryTreeMemoryMap::WriteU8(u8 value, u64 address)
	{
		WriteImpl<u8, std::endian::native>(static_cast<u64>(value), address);
	}

	/*
	 8-bit Read
	*/

	u8 BinaryTreeMemoryMap::ReadU8(u64 address)
	{
		return static_cast<u8>(ReadImpl<u8, std::endian::native>(address));
	}

	/*
	 16-bit Write
	*/

	void BinaryTreeMemoryMap::WriteU16BigEndianImpl(u16 value, u64 address)
	{
		WriteImpl<u16, std::endian::big>(static_cast<u64>(value), address);
	}

	void BinaryTreeMemoryMap::WriteU16LittleEndianImpl(u16 value, u64 address)
	{
		WriteImpl<u16, std::endian::little>(static_cast<u64>(value), address);
	}

	/*
	 16-bit Read
	*/

	u16 BinaryTreeMemoryMap::ReadU16BigEndianImpl(u64 address)
	{
		return static_cast<u16>(ReadImpl<u16, std::endian::big>(address));
	}

	u16 BinaryTreeMemoryMap::ReadU16LittleEndianImpl(u64 address)
	{
		return static_cast<u16>(ReadImpl<u16, std::endian::little>(address));
	}

	/*
	 32-bit Write
	*/

	void BinaryTreeMemoryMap::WriteU32BigEndianImpl(u32 value, u64 address)
	{
		WriteImpl<u32, std::endian::big>(static_cast<u64>(value), address);
	}

	void BinaryTreeMemoryMap::WriteU32LittleEndianImpl(u32 value, u64 address)
	{
		WriteImpl<u32, std::endian::little>(static_cast<u64>(value), address);
	}

	/*
	 32-bit Read
	*/

	u32 BinaryTreeMemoryMap::ReadU32BigEndianImpl(u64 address)
	{
		return static_cast<u32>(ReadImpl<u32, std::endian::big>(address));
	}

	u32 BinaryTreeMemoryMap::ReadU32LittleEndianImpl(u64 address)
	{
		return static_cast<u32>(ReadImpl<u32, std::endian::little>(address));
	}

	/*
	 64-bit Write
	*/

	void BinaryTreeMemoryMap::WriteU64BigEndianImpl(u64 value, u64 address)
	{
		WriteImpl<u64, std::endian::big>(value, address);
	}

	void BinaryTreeMemoryMap::WriteU64LittleEndianImpl(u64 value, u64 address)
	{
		WriteImpl<u64, std::endian::little>(value, address);
	}

	/*
	 64-bit Read
	*/

	u64 BinaryTreeMemoryMap::ReadU64BigEndianImpl(u64 address)
	{
		return ReadImpl<u64, std::endian::big>(address);
	}

	u64 BinaryTreeMemoryMap::ReadU64LittleEndianImpl(u64 address)
	{
		return ReadImpl<u64, std::endian::little>(address);
	}
}
