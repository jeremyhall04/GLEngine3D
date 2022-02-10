#include "player.h"

#include "../utils/camera.h"

Player::Player()
{
	pos = glm::vec3(0, 0, 0);
	dir = glm::vec3(0, 0, 1);
}

Player::Player(PerspectiveCamera* camera)
{
	IM.currSlot = 0;
	IM.slot = new InventorySlot[MAX_INVENTORY_SLOTS];
	IM.slot[IM.currSlot] = { false, BlockType::Dirt, 5 };

	update(camera);
}

Player::~Player()
{
	delete[] IM.slot;
}

void Player::update(PerspectiveCamera* camera)
{
	pos = camera->getPosition();
	dir = camera->getDirection();
}

void Player::pickupBlock(BlockType type)
{
	//char c = 1;
	int firstFreeSlot = -1;
	bool found = false;
	for (int i = 0; i < MAX_INVENTORY_SLOTS; i++)
	{
		if (IM.slot[i].isEmpty && firstFreeSlot == -1)
			firstFreeSlot = i;
		else if (IM.slot[i].blockStackType == type)
		{
			IM.addToStack(i);
			found = true;
			break;
		}
	}

	// create new stack in first free slot
	if (!found)
		IM.initStack(firstFreeSlot, type);
}

BlockType Player::placeBlock() 
{
	BlockType type = BlockType::Default;
	if (!IM.slot[IM.currSlot].isEmpty)
	{
		type = IM.slot[IM.currSlot].blockStackType;
		IM.rmFromStack(IM.currSlot);
	}
	return type;
}

void InventoryManager::initStack(int slotNum, BlockType type)
{
	slot[slotNum].isEmpty = false;
	slot[slotNum].blockStackType = type;
	slot[slotNum].stackSize = 0;
	addToStack(slotNum);
}

void InventoryManager::addToStack(int slotNum)
{
	slot[slotNum].stackSize = slot[slotNum].stackSize + 1 < 64 ? slot[slotNum].stackSize + 1 : slot[slotNum].stackSize;
}

void InventoryManager::rmFromStack(int slotNum)
{
	if (slot[slotNum].stackSize - 1 >= 0)
		slot[slotNum].stackSize--;
	else
		slot[slotNum].stackSize = 0;

	if (slot[slotNum].stackSize == 0)
	{
		slot[slotNum].isEmpty = true;
		slot[slotNum].blockStackType = BlockType::Default;
	}
}
