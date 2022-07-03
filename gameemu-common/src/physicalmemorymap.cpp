#include <game-emu/common/physicalmemorymap.h>

namespace GameEmu::Common
{
	/*
	 8-bit Write
	*/
	void PhysicalMemoryMap::WriteU8(u8 value, u64 address)
	{
		WriteImpl<u8, std::endian::native>(static_cast<u64>(value), address);
	}

	/*
	 8-bit Read
	*/
	u8 PhysicalMemoryMap::ReadU8(u64 address)
	{
		return static_cast<u8>(ReadImpl<u8, std::endian::native>(address));
	}

	/*
	 16-bit Write
	*/
	void PhysicalMemoryMap::WriteU16BigEndianImpl(u16 value, u64 address)
	{
		WriteImpl<u16, std::endian::big>(static_cast<u64>(value), address);
	}

	void PhysicalMemoryMap::WriteU16LittleEndianImpl(u16 value, u64 address)
	{
		WriteImpl<u16, std::endian::little>(static_cast<u64>(value), address);
	}

	/*
	 16-bit Read
	*/
	u16 PhysicalMemoryMap::ReadU16BigEndianImpl(u64 address)
	{
		return static_cast<u16>(ReadImpl<u16, std::endian::big>(address));
	}

	u16 PhysicalMemoryMap::ReadU16LittleEndianImpl(u64 address)
	{
		return static_cast<u16>(ReadImpl<u16, std::endian::little>(address));
	}

	/*
	 32-bit Write
	*/
	void PhysicalMemoryMap::WriteU32BigEndianImpl(u32 value, u64 address)
	{
		WriteImpl<u32, std::endian::big>(static_cast<u64>(value), address);
	}

	void PhysicalMemoryMap::WriteU32LittleEndianImpl(u32 value, u64 address)
	{
		WriteImpl<u32, std::endian::little>(static_cast<u64>(value), address);
	}

	/*
	 32-bit Read
	*/
	u32 PhysicalMemoryMap::ReadU32BigEndianImpl(u64 address)
	{
		return static_cast<u32>(ReadImpl<u32, std::endian::big>(address));
	}

	u32 PhysicalMemoryMap::ReadU32LittleEndianImpl(u64 address)
	{
		return static_cast<u32>(ReadImpl<u32, std::endian::little>(address));
	}

	/*
	 64-bit Write
	*/
	void PhysicalMemoryMap::WriteU64BigEndianImpl(u64 value, u64 address)
	{
		WriteImpl<u64, std::endian::big>(value, address);
	}

	void PhysicalMemoryMap::WriteU64LittleEndianImpl(u64 value, u64 address)
	{
		WriteImpl<u64, std::endian::little>(value, address);
	}

	/*
	 64-bit Read
	*/
	u64 PhysicalMemoryMap::ReadU64BigEndianImpl(u64 address)
	{
		return ReadImpl<u64, std::endian::big>(address);
	}

	u64 PhysicalMemoryMap::ReadU64LittleEndianImpl(u64 address)
	{
		return ReadImpl<u64, std::endian::little>(address);
	}
}
