#ifndef RENDERABLE_3D_H
#define RENDERABLE_3D_H

#include "../GLcommon.h"
#include "../GLcommon_math.h"

#include "../graphics/buffers/buffer.h"
#include "../graphics/buffers/indexbuffer.h"
#include "../graphics/buffers/vertexarray.h"

#include "../graphics/texture/texture.h"

const GLfloat BLOCK_VERTICES[3 * 6 * 6] = {
	// south face (-z)
	1, 0, 0,
	0, 0, 0,
	0, 1, 0,
	1, 0, 0,
	0, 1, 0,
	1, 1, 0,

	// north face (+z)
	0, 0, 1,
	1, 0, 1,
	1, 1, 1,
	0, 0, 1,
	1, 1, 1,
	0, 1, 1,

	1, 0, 1,	// east face (+x)
	1, 0, 0,
	1, 1, 0,
	1, 0, 1,
	1, 1, 0,
	1, 1, 1,

	0, 0, 0,	// west face (-x)
	0, 0, 1,
	0, 1, 1,
	0, 0, 0,
	0, 1, 1,
	0, 1, 0,

	1, 1, 0,	// top face (+y)
	0, 1, 0,
	0, 1, 1,
	1, 1, 0,
	0, 1, 1,
	1, 1, 1,

	1, 0, 1,	// bottom face (-y)
	0, 0, 1,
	0, 0, 0,
	1, 0, 1,
	0, 0, 0,
	1, 0, 0
};

const GLfloat BLOCK_UV[6 * 12] = {
	// south face (-z)
	1, 0,
	0, 0,
	0, 1,
	1, 0,
	0, 1,
	1, 1,

	// north face (+z)
	1, 0,
	0, 0,
	0, 1,
	1, 0,
	0, 1,
	1, 1,

	1, 0,	// east face (+x)
	0, 0,
	0, 1,
	1, 0,
	0, 1,
	1, 1,

	1, 0,	// west face (-x)
	0, 0,
	0, 1,
	1, 0,
	0, 1,
	1, 1,

	1, 0,	// top face (+y)
	0, 0,
	0, 1,
	1, 0,
	0, 1,
	1, 1,

	1, 0,	// bottom face (-y)
	0, 0,
	0, 1,
	1, 0,
	0, 1,
	1, 1
};

const float BLOCK_NORMALS[6 * 18] = {
	 0,  0, -1, // South
	 0,  0, -1,
	 0,  0, -1,
	 0,  0, -1,
	 0,  0, -1,
	 0,  0, -1,

	 0,  0,  1, // North
	 0,  0,  1,
	 0,  0,  1,
	 0,  0,  1,
	 0,  0,  1,
	 0,  0,  1,

	 1,  0,  0, // East
	 1,  0,  0,
	 1,  0,  0,
	 1,  0,  0,
	 1,  0,  0,
	 1,  0,  0,

	-1,  0,  0, // West
	-1,  0,  0,
	-1,  0,  0,
	-1,  0,  0,
	-1,  0,  0,
	-1,  0,  0,

	 0,  1,  0, // Top
	 0,  1,  0,
	 0,  1,  0,
	 0,  1,  0,
	 0,  1,  0,
	 0,  1,  0,

	 0, -1,  0, // Bottom
	 0, -1,  0,
	 0, -1,  0,
	 0, -1,  0,
	 0, -1,  0,
	 0, -1,  0,
};

const GLfloat BLOCK_INDEXED_VERTICES[24] = {
	0, 0, 0,
	1, 0, 0,
	1, 1, 0,
	0, 1, 0,

	0, 0, 1,
	1, 0, 1,
	1, 1, 1,
	0, 1, 1
};

const GLushort BLOCK_INDICES[36] = {
	1, 0, 3, 1, 3, 2, // north (-z)
	4, 5, 6, 4, 6, 7, // south (+z)
	5, 1, 2, 5, 2, 6, // east (+x)
	0, 4, 7, 0, 7, 3, // west (-x)
	2, 3, 7, 2, 7, 6, // top (+y)
	5, 4, 0, 5, 0, 1, // bottom (-y)
};

//const GLfloat BLOCK_UV[16] = {
//	0, 0,
//	1, 0,
//	1, 1,
//	0, 1,
//	0, 0,
//	1, 0,
//	1, 1,
//	0, 1,
//};

enum class FaceDirection
{
	XNeg,
	XPos,
	YNeg,
	YPos,
	ZNeg,
	ZPos
};

enum class BlockType {
	Default = 0,
	Air = 1,
	Dirt = 2,
	Grass = 3,
	Stone = 5
};


// new vertexData contain position, normal, and tid -> can store in a 32 bit binary integer
// vertex = 3 floats = 12 bytes
// texture = 3 floats = 12 bytes (for texture coordinate of an atlas) || vec3(uv.x, uv.y, tid) for example
// normal = 1 float ?? = 4 bytes
struct VertexData3D
{
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 uv;
	float tid;
	GLuint color;
};

struct VertexData
{
	glm::vec3 position;	// 3 floats / vertex = 12 bytes / vertex
	glm::vec2 uv;
	float tid;
	float light;	// dependant on the direction the block is facing, reduces to 3 bits to store the lighting value
	// Top face = 1.0		* 5 = 5	= 101
	// East/West = 0.8		* 5 = 4 = 100
	// North/South = 0.6	* 5 = 3 = 011
	// Bottom = 0.4			* 5 = 2 = 010
};

class Renderable3D
{
public:
	bool isActive = true;
	// ZN, ZP, XP, XN, YP, YN
	bool renderFace[6];

private:
	glm::vec3 m_Position;
	glm::vec3 m_Size;
	glm::vec4 m_Color;
	BlockType m_TypeID;

public:
	Renderable3D()
	{
	};
	Renderable3D(glm::vec3 position, glm::vec3 size, glm::vec4 color, BlockType typeID)
		: m_Position(position), m_Size(size), m_Color(color), m_TypeID(typeID)
	{
		if (m_TypeID == BlockType::Air)
			isActive = false;
	}
	virtual ~Renderable3D() {}

	inline const glm::vec3 getPosition() const { return m_Position; };
	inline const glm::vec3 getSize()	 const { return m_Size; };
	inline const glm::vec4 getColor()	 const { return m_Color; };
	inline const GLuint getTIDfromBlockType() const { return (GLuint)m_TypeID; };
	inline void setType(BlockType type) { m_TypeID = type; };
	inline const BlockType getType() const { return m_TypeID; };
};

#endif // !RENDERABLE_3D_H

