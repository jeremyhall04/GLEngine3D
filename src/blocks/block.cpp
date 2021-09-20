#include "block.h"

Blocks WorldBlocks;

Block::Block()
{
}

Block::Block(float x, float y, float z, float cube_size, const glm::vec4 color)
	: Renderable3D(glm::vec4(x, y, z, 1.0f), glm::vec3(cube_size, cube_size, cube_size), color, BlockType::_Default), m_ID((int)BlockType::_Default)
{
}
 
Block::Block(float x, float y, float z, float cube_size, BlockType blockType)
	: Renderable3D(glm::vec4(x, y, z, 1.0f), glm::vec3(cube_size, cube_size, cube_size), glm::vec4(1, 0, 1, 1), blockType), m_ID((int)blockType)
{

}

void add_to_blocks(Block& block)
{
	WorldBlocks.blockCount++;
	WorldBlocks.blocks[WorldBlocks.blockCount] = new Block;
	WorldBlocks.blocks[WorldBlocks.blockCount] = &block;
}

void remove_from_blocks(GLuint blockIndex)
{
	if (WorldBlocks.blocks[blockIndex] != nullptr)
	{
		delete WorldBlocks.blocks[blockIndex];
		WorldBlocks.blockCount--;
	}
	else
		printf("\nERROR::BLOCK::remove_from_blocks(): blocks[blockIndex] = nullptr");
}