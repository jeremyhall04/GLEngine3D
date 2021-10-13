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
	Air = 0,
	Dirt = 1,
	Grass = 2,
	Stone = 4
};

struct VertexData3D
{
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 uv;
	float tid;
	GLuint color;
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
	inline const BlockType getType() const { return m_TypeID; };
	//virtual void setFacesToRender(bool NX, bool PX, bool NY, bool PY, bool NZ, bool PZ) = 0;
	//inline const int getTID()		 const { return m_Texture ? m_Texture->getID() : 0; };
};

#endif // !RENDERABLE_3D_H

