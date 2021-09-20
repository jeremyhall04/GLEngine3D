#ifndef RENDERABLE_3D_H
#define RENDERABLE_3D_H

#include "../GLcommon.h"
#include "../GLcommon_math.h"

#include "../graphics/buffers/buffer.h"
#include "../graphics/buffers/indexbuffer.h"
#include "../graphics/buffers/vertexarray.h"

#include "../graphics/texture/texture.h"
#include "../utils/direction.h"

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
	1, 0, 0,
};

const GLfloat BLOCK_UV[3 * 36] = {
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

const int BLOCK_NORMALS[18] = {
	 0,  0,  1, // North
	 0,  0, -1, // South
	 1,  0,  0, // East
	-1,  0,  0, // West
	 0,  1,  0, // Top
	 0, -1,  0  // Bottom
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

enum class BlockType {
	_Default = 0,
	Grass = 1,
	Stone = 2
};

struct VertexData3D
{
	glm::vec4 vertex;
	glm::vec2 uv;
	float tid;
	GLuint color;
};

class Renderable3D
{
private:
	glm::vec4 m_Position;
	glm::vec3 m_Size;
	glm::vec4 m_Color;
	BlockType m_TypeID;

public:
	GLboolean isActive = true;
	Renderable3D()
	{
	};

	Renderable3D(glm::vec4 position, glm::vec3 size, glm::vec4 color, BlockType typeID)
		: m_Position(position), m_Size(size), m_Color(color), m_TypeID(typeID)
	{
	}
	virtual ~Renderable3D() {}

	inline const glm::vec4 getPosition() const { return m_Position; };
	inline const glm::vec3 getSize()	 const { return m_Size; };
	inline const glm::vec4 getColor()	 const { return m_Color; };
	inline const GLuint getTIDfromBlockType() const { return (GLuint)m_TypeID; };
	//inline const int getTID()		 const { return m_Texture ? m_Texture->getID() : 0; };
};

#endif // !RENDERABLE_3D_H

