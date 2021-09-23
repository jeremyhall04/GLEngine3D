#include "block.h"

Block::Block()
{
}

Block::Block(float x, float y, float z, float cube_size, const glm::vec4 color)
	: Renderable3D(glm::vec3(x, y, z), glm::vec3(cube_size, cube_size, cube_size), color, BlockType::_Default), m_ID((int)BlockType::_Default)
{
	m_isActive = true;
}
 
Block::Block(float x, float y, float z, float cube_size, BlockType blockType)
	: Renderable3D(glm::vec3(x, y, z), glm::vec3(cube_size, cube_size, cube_size), glm::vec4(1, 0, 1, 1), blockType), m_ID((int)blockType)
{
	m_isActive = true;
}

void add_to_blocks(Block& block)
{
}

void remove_from_blocks(GLuint blockIndex)
{
}