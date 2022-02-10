#ifndef PLAYER_H
#define PLAYER_H

#include "../GLcommon.h"
#include "../GLcommon_math.h"
#include "ray.h"
#include "../blocks/block.h"
#include <vector>

const int MAX_INVENTORY_SLOTS = 3;

class PerspectiveCamera;

struct InventorySlot
{
	bool isEmpty = true;
	BlockType blockStackType;
	unsigned int stackSize;
};

struct InventoryManager
{
	InventorySlot* slot;
	unsigned int currSlot;
	//unsigned char activeSlots;

	void initStack(int slotNum, BlockType type);
	void emptyStack(int slotNum);
	void addToStack(int slot);
	void rmFromStack(int slot);
};

class Player
{
private:
	glm::vec3 pos, dir;
	float rayLength = 4.0f;
	InventoryManager IM;

public:
	Ray ray;

public:
	Player();
	Player(PerspectiveCamera* camera);
	~Player();

	void update(PerspectiveCamera* camera);
	const glm::vec3& getPos() const { return pos; };
	const glm::vec3& getDir() const { return dir; };
	void setActiveSlot(int slot) { IM.currSlot = slot; };
	void pickupBlock(BlockType type);
	BlockType placeBlock();
	InventoryManager* getInventory() { return &IM; };
	//void setActiveInventorySlot(int slot) { activeSlot = slot; };
};

#endif // !PLAYER_H
