#include <game-emu/common/physicalmemorymap.h>

namespace GameEmu::Common
{
	const std::shared_ptr<PhysicalMemoryMap::Entry> BinaryTreeMemoryMap::Map(u8* hostReadMemory, u8* hostWriteMemory, u64 size, u64 baseAddress,
		Entry::ReadEventFunction readEvent, Entry::WriteEventFunction writeEvent)
	{
		// Create a new entry.
		std::shared_ptr<Entry> entry = std::make_shared<Entry>(baseAddress,
			hostReadMemory, hostWriteMemory, size, readEvent, writeEvent);
		
		// If the root is null, then this is the first entry.
		if (!root)
		{
			root = std::make_shared<Node>(entry, nullptr);
			return entry;
		}
		
		// Otherwise, find the correct location to insert the entry.
		std::shared_ptr<Node> current = root;
		while (true)
		{
			// If the entry is smaller than the current entry, then insert it to the left.
			if (entry->address < current->entry->address)
			{
				if (!current->left)
				{
					current->left = std::make_shared<Node>(entry, current);
					return entry;
				}
				current = current->left;
			}
			else // Otherwise, insert it to the right.
			{
				if (!current->right)
				{
					current->right = std::make_shared<Node>(entry, current);
					return entry;
				}
				current = current->right;
			}
		}
		return nullptr;
	}

	void BinaryTreeMemoryMap::Unmap(const std::shared_ptr<Entry>& entry)
	{
		// If the root is null, then there is nothing to delete.
		if (!root)
			return;
		
		// Otherwise, find the entry to delete.
		std::shared_ptr<Node> current = root;
		while (true)
		{
			// If the entry is smaller than the current entry, then move to the left.
			if (entry->address < current->entry->address)
			{
				if (!current->left)
					return;
				if (current->left->entry == entry)
					break;
				current = current->left;
			}
			else // Otherwise, move to the right.
			{
				if (!current->right)
					return;
				if (current->right->entry == entry)
					break;
				current = current->right;
			}
		}
		
		// If the entry has no children, then delete it.
		if (!current->left && !current->right)
		{
			if (current == root)
				root = nullptr;
			else
			{
				if (current == current->parent->left)
					current->parent->left = nullptr;
				else
					current->parent->right = nullptr;
			}
			return;
		}
		
		// If the entry has one child, then replace it with its child.
		if (current->right)
		{
			if (current == root)
				root = current->right;
			else
			{
				current->right->parent = current->parent;
				if (current == current->parent->left)
					current->parent->left = current->right;
				else
					current->parent->right = current->right;
			}
		}
		else if (current->left)
		{
			if (current == root)
				root = current->left;
			else
			{
				current->left->parent = current->parent;
				if (current == current->parent->left)
					current->parent->left = current->left;
				else
					current->parent->right = current->left;
			}
		}
		// Otherwise if the entry has two children, then replace it with its successor (the leftmost smallest node in the right subtree).
		else if (current->right && current->left)
		{
			std::shared_ptr<Node> smallest = current->right;
			while (smallest->left)
				smallest = smallest->left;

			if (smallest->right)
			{
				smallest->right->parent = smallest->parent;
				if (smallest == smallest->parent->left)
					smallest->parent->left = smallest->right;
				else
					smallest->parent->right = smallest->right;
			}
			else if (smallest->left)
			{
				smallest->left->parent = smallest->parent;
				if (smallest == smallest->parent->left)
					smallest->parent->left = smallest->left;
				else
					smallest->parent->right = smallest->left;
			}

			current->entry = smallest->entry;
		}
	}

	const std::shared_ptr<PhysicalMemoryMap::Entry> BinaryTreeMemoryMap::FindEntry(u64 address)
	{
		std::shared_ptr<Node> node = root;
		while (node)
		{
			if (address < node->entry->address)
				node = node->left;
			else if (address >= node->entry->address + node->entry->size)
				node = node->right;
			else
				return node->entry;
		}
		return nullptr;
	}
}
